#include "TkUtil/FileRedirection.h"

#include "TkUtil/UTF8String.h"
#include "TkUtil/File.h"
#include "TkUtil/Exception.h"

#include <fstream>
#include <stdexcept>

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

static const IN_UTIL Charset	charset ("àéèùô");

#define CHECK_FILE_DESCRIPTOR(fd)                                        \
if (-1 == fd)                                                            \
{                                                                        \
	UTF8String	error (charset);                                     \
	error << "Echec de la redirection d'un flux : "                      \
	      << strerror (errno) << ".";                                    \
	errno	= 0;                                                         \
	throw Exception (error);                                             \
}


USING_STD


BEGIN_NAMESPACE_UTIL


static const int	r_flags	= O_RDONLY;
static const int	w_flags	= O_WRONLY | O_CREAT | O_TRUNC;

FileRedirection::FileRedirection (
			FILE* fileDesc, const string& fileName, bool always, bool toRemove)
	: _fileDesc (fileDesc), _backupFileDesc (-1), 
	  _fileName (fileName), _backupFileName ( ), _removeAtCompletion (toRemove)
{
	if (NULL == _fileDesc)
	{
		UTF8String	error (charset);
		error << "Descripteur de flux nul pour le fichier à rediriger dans le "
		      << "fichier " << fileName << ".";
		throw Exception (error);
	}	// if (NULL == _fileDesc)

	const int	fd	= fileno (_fileDesc);
	// Redirection en lecture ou en écriture ?
	int	flag	= fcntl (fd, F_GETFL);
	CHECK_FILE_DESCRIPTOR (flag)
	int	mode	= 0 != (flag & O_RDONLY) ? r_flags : w_flags;

	// Conserver une copie du flux initial afin de pouvoir le réutiliser après
	// destruction de cette instance :
	if (false == always)
	{
		_backupFileName	= File::createTemporaryName ("fileRedirect", true);
		int	tmp	= open (_backupFileName.c_str ( ), mode, 0600);
		CHECK_FILE_DESCRIPTOR (tmp)
		_backupFileDesc	= dup2 (fd, tmp);
		CHECK_FILE_DESCRIPTOR (_backupFileDesc)
//		close (tmp);	// -> flux initial non recuperable
	}	// if (false == always)

	// Creation du flux de redirection :
	int	rfd	= open (fileName.c_str ( ), mode, 0600);
	CHECK_FILE_DESCRIPTOR (rfd);

	// On redirige le flux dans celui créé :
	int	redirectedFileDesc	= dup2 (rfd, fd);
	CHECK_FILE_DESCRIPTOR (redirectedFileDesc)
	close (rfd);
}	// FileRedirection::FileRedirection


FileRedirection::FileRedirection (const FileRedirection&)
{
	assert (0 && "FileRedirection copy constructor is not allowed.");
}	// FileRedirection::FileRedirection (const FileRedirection&)


FileRedirection& FileRedirection::operator = (const FileRedirection&)
{
	assert (0 && "FileRedirection operator = is not allowed.");
	return *this;
}	// FileRedirection::operator = (const FileRedirection&)


FileRedirection::~FileRedirection ( )
{
	terminate ( );

	if (true == _removeAtCompletion)
		unlink (_fileName.c_str ( ));
}	// FileRedirection::~FileRedirection


void FileRedirection::terminate ( )
{
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
}	// FileRedirection::terminate


string FileRedirection::getText ( ) const
{
	UTF8String	errorMsg (charset), text (charset);
	errorMsg << "Erreur de lecture dans le fichier " << _fileName
	         << " :" << '\n';

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
				throw Exception (
					UTF8String ("Impossibilité de lire la ligne.", charset));

			text << buffer;	// v 2.28.0
			lineCount++;
		}	// while ((true == stream.good ( )) && (false == stream.eof ( )))
	}
	catch (const std::exception& exc)
	{
		errorMsg << exc.what ( ) << '\n'
		         << "en ligne " << (unsigned long)lineCount << ".";
		throw Exception (errorMsg);
	}
	catch (...)
	{
		errorMsg << "Erreur non documentée." << '\n'
		         << "en ligne " << (unsigned long)lineCount << ".";
		throw Exception (errorMsg);
	}

//	return text.iso ( );
	return text.utf8 ( );	// v 5.6.1
}	// FileRedirection::getText


END_NAMESPACE_UTIL
