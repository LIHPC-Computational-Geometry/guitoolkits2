#include "TkUtil/DuplicatedStream.h"

#include "TkUtil/UTF8String.h"
#include "TkUtil/File.h"
#include "TkUtil/Exception.h"

#include <fstream>
#include <stdexcept>

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>


// Passe à true lorsque le processus père tue le processus fils qui lit et
// redirige le flux considéré. Cela a pour effet alors d'areter la lecture et
// redirection dans le processus fils, puis de de l'arrêter.
bool	stopped	= false;

static void completionCalled (int sigNum, siginfo_t*, void*)
{
	if (SIGUSR1 == sigNum)
		stopped	= true;
}	// completionCalled

static const IN_UTIL	Charset	charset ("àéèùô");

#define CHECK_FILE_DESCRIPTOR(fd)                                        \
if (-1 == fd)                                                            \
{                                                                        \
	UTF8String	error (charset);                                         \
	error << "Echec de la redirection d'un flux : "                      \
	      << strerror (errno) << ".";                                    \
	errno	= 0;                                                         \
	throw Exception (error);                                             \
}


USING_STD


BEGIN_NAMESPACE_UTIL


static const int	r_flags	= O_RDONLY;
//static const int	w_flags	= O_WRONLY | O_CREAT | O_TRUNC;
//static const int	w_flags	= O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT | O_NONBLOCK | O_NDELAY;
static const int	w_flags	= O_WRONLY | O_CREAT | O_TRUNC;

DuplicatedStream::DuplicatedStream (
			FILE* fileDesc, const string& fileName)
	: _fileDesc (fileDesc), _backupFileDesc (-1), 
	  _fileName (fileName), _backupFileName ( ), _childPid ((pid_t)-1)
{
	if (NULL == _fileDesc)
	{
		UTF8String	error (charset);
		error << "Descripteur de flux nul pour le fichier à copier dans le "
		      << "fichier " << fileName << ".";
		throw Exception (error);
	}	// if (NULL == _fileDesc)

	const int	fd	= fileno (_fileDesc);
	// Duplication en lecture ou en écriture ?
	// NON TESTE EN LECTURE
	int	flag	= fcntl (fd, F_GETFL);
	CHECK_FILE_DESCRIPTOR (flag)
	int	mode	= 0 != (flag & O_RDONLY) ? r_flags : w_flags;

	// Conserver une copie du flux initial afin de pouvoir le réutiliser après
	// destruction de cette instance :
	_backupFileName	= File::createTemporaryName ("fileRedirect", true);
	int	tmp	= open (_backupFileName.c_str ( ), mode, 0600);
	CHECK_FILE_DESCRIPTOR (tmp)
	_backupFileDesc	= dup2 (fd, tmp);
	CHECK_FILE_DESCRIPTOR (_backupFileDesc)

	// Creation du flux de redirection :
	int	rfd	= open (fileName.c_str ( ), mode, 0600);
	CHECK_FILE_DESCRIPTOR (rfd);

	// On redirige le flux dans celui créé :
	int	redirectedFileDesc	= dup2 (rfd, fd);
	CHECK_FILE_DESCRIPTOR (redirectedFileDesc)
	close (rfd);

	_childPid	= fork ( );
	switch (_childPid)
	{
		case	-1	:
		{
			UTF8String	error (charset);
			error << "Erreur de fork : " << strerror (errno);
			throw Exception (error);
		}
		case	0	:	// Fils
		{
			// On positionne un handler afin de savoir quand la
			// lecture/redirection doit s'arrêter.
			sigset_t			sigmask;
			struct sigaction	action, oldaction;
			sigemptyset (&sigmask);
			errno		= 0;
			sigemptyset (&action.sa_mask);
			action.sa_sigaction		= completionCalled;
			action.sa_flags			= SA_RESTART | SA_SIGINFO;
			if (-1 == sigaction (SIGUSR1, &action, &oldaction))
			{
				ConsoleOutput::cout ( )
				     << "Impossibilité de récupérer le signal de terminaison "
				     << " venant du processus père. Flux backup impossible."
				     << co_endl;
				exit (-1);
			}	// if (-1 == sigaction (SIGUSR1, &action, &oldaction))

			FILE*	redirectFile	= fopen (fileName.c_str ( ), "r");
			if (NULL == redirectFile)
			{
				ConsoleOutput::cout ( ) << "Flux backup invalide." << co_endl;
				exit (-1);
			}

			char	buffer [10001];
			while ((false == stopped) || (0 == feof (redirectFile)))
			{
				memset (buffer, '\0', 10001);
				fsync (fileno (redirectFile));
				fread (buffer, sizeof (char), 10000, redirectFile);
				const size_t	count	= strlen (buffer);
//if (0 != count)
//	fprintf (stderr, "BUFFER:%s COUNT = %d\n", buffer, (int)count);
				if (0 != count)
				{
					write (_backupFileDesc, buffer, count);
					fsync (_backupFileDesc);
				}	// if (0 != count)
			}	// while (false == stopped)
			close (_backupFileDesc);
			fclose (redirectFile);

			exit (0);
		}	// case 0 (fils)
		default	:	// Père
		{	// Rend la main à la fonction appelante.
		}	// default (père)
	}	// switch (_childPid)
}	// DuplicatedStream::DuplicatedStream


DuplicatedStream::DuplicatedStream (const DuplicatedStream&)
{
	assert (0 && "DuplicatedStream copy constructor is not allowed.");
}	// DuplicatedStream::DuplicatedStream (const DuplicatedStream&)


DuplicatedStream& DuplicatedStream::operator = (const DuplicatedStream&)
{
	assert (0 && "DuplicatedStream operator = is not allowed.");
	return *this;
}	// DuplicatedStream::operator = (const DuplicatedStream&)


DuplicatedStream::~DuplicatedStream ( )
{
	terminate ( );

	unlink (_fileName.c_str ( ));
}	// DuplicatedStream::~DuplicatedStream


void DuplicatedStream::terminate ( )
{
	if (NULL != _fileDesc)
		fflush (_fileDesc);

	// Provoquer la terminaison du processus fils :
	kill (_childPid, SIGUSR1);

	// Attente que le processus fils soit fini :
	int		status		= 0;
	bool	completed	= false;
	while (false == completed)
	{
		waitpid (_childPid, &status, 0);
//		if (0 != WIFEXITED (status))
		if (0 == WIFCONTINUED (status))
			completed	= true;
	}	// while (false == completed)

	if (-1 != _backupFileDesc)
	{
		assert (NULL != _fileDesc);
		const int	fd	= fileno (_fileDesc);
		fflush (_fileDesc);
		_fileDesc	= NULL;
		int	res	= dup2 (_backupFileDesc, fd);
// Sous RHELWS_3_i686 leve une exception : Bad file descriptor, et pourtant ca
// marche ...
//		CHECK_FILE_DESCRIPTOR (res)
		close (_backupFileDesc);
		_backupFileDesc		= -1;
		unlink (_backupFileName.c_str ( ));
	}	// if (-1 != _backupFileDesc)
}	// DuplicatedStream::terminate


string DuplicatedStream::getText ( ) const
{
	UTF8String	errorMsg (charset), text (charset);
	errorMsg << "Erreur de lecture dans le fichier " << _fileName << " :"
	         << '\n';

	size_t	lineCount	= 1;
	try
	{
		ifstream	stream (_fileName.c_str ( ));
		if ((false == stream.good ( )) && (false == stream.eof ( )))
		{
			errorMsg << "Flux invalide.";
			throw Exception (errorMsg);
		}

		char	buffer [10001];
		while ((true == stream.good ( )) && (false == stream.eof ( )))
		{
//			stream.getline (buffer, 10000);	// v 2.28.0
			memset (buffer, '\0', 10001);
			stream.read (buffer, 10000);	// v 2.28.0
			int	count	= stream.gcount ( );	// v 2.28.0
			if ((true == stream.fail ( )) && (false == stream.eof ( )))
				throw Exception (UTF8String ("Impossibilité de lire la ligne.", charset));

//			text << buffer << '\n';	// v 2.28.0
			text << buffer;	// v 2.28.0
			lineCount++;
		}	// while ((true == stream.good ( )) && (false == stream.eof ( )))
	}
	catch (const std::exception& exc)
	{
		errorMsg << exc.what ( ) << '\n'
		         << "en ligne " << (unsigned long)lineCount << ".";
//		throw Exception (errorMsg);
		throw Exception (UTF8String (errorMsg, charset));	// v 5.6.1
	}
	catch (...)
	{
		errorMsg << "Erreur non documentée." << '\n'
		         << "en ligne " << (unsigned long)lineCount << ".";
//		throw Exception (errorMsg);
		throw Exception (UTF8String (errorMsg, charset));	// v 5.6.1
	}

	return text.iso ( );
}	// DuplicatedStream::getText


END_NAMESPACE_UTIL
