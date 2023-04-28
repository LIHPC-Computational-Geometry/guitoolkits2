#include "TkUtil/RemoteProcess.h"
#include "TkUtil/UTF8String.h"
#include "TkUtil/Exception.h"
#include "TkUtil/MemoryError.h"
#include "TkUtil/AutoArrayPtr.h"

#include <assert.h>

#include <iostream>
//#include <strstream>

USING_UTIL
USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");

// ===========================================================================
//                           Classe RemoteProcess
// ===========================================================================


string RemoteProcess::remoteShell ("ssh");
string RemoteProcess::launcher ("launcher");


RemoteProcess::RemoteProcess (const string& host, const string& processName)
	: Process (remoteShell), _host (host), _remoteProcess (processName), 
	  _remotePath (""), _remotePid ((pid_t)-1), _serverWaitingMessage ("")
{
// ATTENTION : A AJOUTER pour eviter appels bloquants sur une machine tombée.
//getOptions ( ).addOption ("-o");
//getOptions ( ).addOption ("ConnectTimeout=60");
	getOptions ( ).addOption (_host);
	getOptions ( ).addOption (launcher);
	getOptions ( ).addOption (_remoteProcess);
	enableChildToSonCommunications ( );
}	// RemoteProcess::RemoteProcess


RemoteProcess::RemoteProcess (const string& host, const string& processName,
                              const vector<string>& rshOptions)
	: Process (remoteShell), _host (host), _remoteProcess (processName), 
	  _remotePath (""), _remotePid ((pid_t)-1), _serverWaitingMessage ("")
{
// ATTENTION : A AJOUTER pour eviter appels bloquants sur une machine tombée.
//getOptions ( ).addOption ("-o");
//getOptions ( ).addOption ("ConnectTimeout=60");
//cout << "NEW REMOTE PROCESS CONSTRUCTOR : ";
	for (vector<string>::const_iterator itrsho = rshOptions.begin ( );
	     rshOptions.end ( ) != itrsho; itrsho++)
		getOptions ( ).addOption (*itrsho);
//cout << _host << " " << launcher << " " << _remoteProcess << endl;*/
	getOptions ( ).addOption (_host);
	getOptions ( ).addOption (launcher);
	getOptions ( ).addOption (_remoteProcess);
	enableChildToSonCommunications ( );
}	// RemoteProcess::RemoteProcess


RemoteProcess::RemoteProcess (const RemoteProcess&)
	: Process ("")
{
	assert (0 && "RemoteProcess::RemoteProcess (const RemoteProcess&) is not allowed.");
}	// RemoteProcess::RemoteProcess (const RemoteProcess&)


RemoteProcess& RemoteProcess::operator = (const RemoteProcess&)
{
	assert (0 && "RemoteProcess::operator = (const RemoteProcess&) is not allowed.");
	return *this;
}	// RemoteProcess::RemoteProcess (const RemoteProcess&)


RemoteProcess::~RemoteProcess ( )
{                                   
}	// RemoteProcess::~RemoteProcess


string RemoteProcess::getRemotePath ( )
{
	if (0 == _remotePath.length ( ))
	{
		UTF8String	message (charset);
		message << "Chemin d'accès à l'exécutable " << getRemoteProcess ( )
		        << " sur la machine " << getHostName ( ) 
		        << " encore inconnu.";
		setErrorMessage (message.utf8 ( ).c_str ( ));
		throw Exception (message);
	}

	return _remotePath;
}	// RemoteProcess::getRemotePath


void RemoteProcess::execute (bool autoDelete)
{
	// Eviter une autodestruction suite a un processus fils tres court :
	setWaited (true);

	const string	remoteProcess	= getRemoteProcess ( );
	const string	hostName		= getHostName ( );
	const string	srvWaitingMsg	= getServerWaitingMessage ( );
	int*			fifo			= getPipeDescriptors ( );
	Process::execute (false);
	setWaited (!autoDelete);

	if (0 != getPid ( ))	// Process parent
	{
// 1.25.0 :
// Si le process doit s'auto-detruire, et que ca n'est pas un serveur, on quitte
// maintenant cette fonction. Il peut etre très rapide et peut être déjà
// terminé, et cet objet détruit ...
		if ((true == autoDelete) && (0 == srvWaitingMsg.length ( ))) // 1.25.0
			return;
// istrstream et istringstream semblent perturber différents compilos et/ou
// purify, sous Sun et SGI.
// => passage par sscanf ...
// ATTENTION : appel à getChildLine bloquant si absence de timeout sur le
// shell.

// Dans de rares cas le processus fils écrit dans stdout avant que launcher
// n'ai eu le temps d´écrire le nom de la machine hôte, le pid et le chemin
// complet de l'exécutable
// => V 1.27.0 : ces infos sont mémorisées afin d'être accessibles par la
// suite.
		bool	infos	= false;	// A t'on les infos sur le processus fils ?	
		string	lines;
		while (false == infos)
		{
			const string		line		= getChildLine ( );
			const size_t		lineLength	= line.length ( );
			AutoArrayPtr<char>	remoteHostName (lineLength + 1);
			AutoArrayPtr<char>	fileName (lineLength + 1);
			CHECK_NULL_PTR_ERROR (remoteHostName.get ( ))
			CHECK_NULL_PTR_ERROR (fileName.get ( ))
//			istrstream	childInfo (line.c_str ( ));	// CORRIGE BOGUE SGI
//			string	remoteHostName, fileName;
			// reserve : purify détecte une écriture en zone non allouée
//			remoteHostName.reserve (line.length ( ) + 1);
//			fileName.reserve (line.length ( ) + 1);
//			childInfo >> remoteHostName >> _remotePid >> fileName;
			unsigned long	pid	= (unsigned long)-1;
			if ((3 != sscanf (line.c_str ( ), "%s %lu %s",
					remoteHostName.get ( ), &pid, fileName.get ( )) ||
			    (pid_t(-1) == pid)))
			{
				lines	+= line + '\n';
				continue;
			}
			infos	= true;
			if (0 != lines.length ( ))
				appendChildBuffer (lines, false);
			_remotePid	= (pid_t)pid;
			setRemotePath (fileName.get ( ));
			_host	= remoteHostName.get ( );
		}	// while (false == infos)
/* ======================================================================
//		if ((pid_t(-1) == _remotePid) || (true == childInfo.bad ( )) || 
//		    (true == childInfo.fail ( )))
		{
			UTF8String	message;
			message << "Il est impossible d'exécuter le process " 
			        << remoteProcess << " sur la machine " << hostName << " : "
			        << line;
			throw Exception (message);
		}
====================================================================== */

		// Faut il attendre un message du processus fils indiquant que c'est
		// un serveur en attente de requetes ?
		lines	= "";
		if (0 != srvWaitingMsg.length ( ))
		{
			bool	waiting		= false;
			while (false == waiting)
			{
				const string	srvBuffer	= getChildLine ( );
//				if (0 == srvBuffer.length ( ))
//					break;
				if (srvWaitingMsg == srvBuffer)
					waiting	= true;
				else
					lines	+= string (srvBuffer) + "\n";
//				cout << srvBuffer << endl;
			}	// while (false == waiting)
			if (0 != lines.length ( ))
				appendChildBuffer (lines, false);
		}	// if (0 != srvWaitingMsg.length ( ))
	}
}	// RemoteProcess::execute


int RemoteProcess::wait ( )
{
	if (true == isCompleted ( ))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Impossibilité d'exécuter le processus " << getName ( )
		         << " : processus terminé.";
		throw Exception (errorMsg);
	}	// if (true == isCompleted ( ))

	setWaited (true);
	try
	{
		while (false == isCompleted ( ))
		{
			string	line = getChildLine ( );
			if (0 == line.length ( ))
				break;
			ConsoleOutput::cout ( ) << line << co_endl;
		}	// while (false == isCompleted ( ))
	}
	catch (...)
	{
	}

	return getCompletionCode ( );
}	// RemoteProcess::wait


void RemoteProcess::kill ( )
{
	if (true == isCompleted ( ))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Impossibilité de tuer le processus " << getName ( )
		         << " : processus terminé.";
		throw Exception (errorMsg);
	}	// if (true == isCompleted ( ))

	RemoteProcess*	killer	= new RemoteProcess (getHostName ( ), "kill");
	killer->getOptions ( ).addOption ("-9");
	UTF8String	pid (charset);
	pid << (unsigned long)getRemotePid ( );
	killer->getOptions ( ).addOption (pid.ascii ( ));
	try
	{
		killer->execute (false);
		killer->wait ( );
		delete killer;
	}
	catch (...)
	{
		delete killer;
		throw;
	}

	try
	{
		Process::kill ( );
	}
	catch (...)
	{
	}
}	// RemoteProcess::kill ( )


void RemoteProcess::setRemotePath (const string& path)
{
	_remotePath	= path;
}	// RemoteProcess::setRemotePath


void RemoteProcess::setServerWaitingMessage (const string& waitingMsg)
{
	_serverWaitingMessage	= waitingMsg;
}	// RemoteProcess::setServerWaitingMessage


END_NAMESPACE_UTIL
