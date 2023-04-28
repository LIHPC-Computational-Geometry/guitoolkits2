/**
 * Lance le processus fils avec les arguments reçus.
 * Ecrit sur la sortie standard le nom de la machine distante, le pid et le nom
 * complet du processus fils en cas de succès, puis la sortie standard du 
 * processus fils.
 * En cas d'échec, écrit un message d'erreur.
 */
#include "TkUtil/Process.h"
#include "TkUtil/NetworkData.h"
#include "TkUtil/Exception.h"

#include <iostream>
#include <fstream>
#include <strstream>
#include <stdexcept>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <errno.h>


USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");


int main (int argc, char* argv [], char* envp[])
{
	int	delay	= 0;
	if (2 > argc)
	{
		cout << "Syntax : " << argv [0]
		     << " [-pipeDelay delai] process option1 ... optionn" 
		     << endl
		     << "-pipeDelay     : "
		     << "éventuel délai d'attente après la fin du processus lancé "
		     << "de récupération des flux sortants."
		     << endl;
		return -1;
	}	// if (2 > argc))

	try
	{
		int			processArg	= 1;
		if ((3 <= argc) && (0 == (strcmp ("-pipeDelay", argv [1]))))
		{
			processArg	+= 2;
			delay	= atoi (argv [2]);
		}	// if ((3 <= argc) && (0 == (strcmp ("-pipeDelay", argv [1])))

		const string	hostName	= NetworkData::getCurrentFullHostName ( );
		int				firstArg	= processArg + 1;
		Process::initialize (envp);
		Process*	process		= new Process (argv [processArg]);
		for (int i = firstArg; i < argc; i++)
			process->getOptions ( ).addOption (argv [i]);
//		process->enableChildToSonCommunications (true);	// v 2.19.1
		process->execute (false);
		pid_t	pid	= (pid_t)-1;
		string	path;
		pid	= process->getPid ( );
		path	= process->getPath ( );
		cout << hostName << ' ' << pid << ' ' << path << endl;

		// Attente de la terminaison :
		int		status	= process->wait ( );	// v 2.25.1
		bool	done	= false;
		// v 2.19.1 : lecture des sorties du processus fils et réécriture dans
		// stdout :
/*
		while (false == done)
		{
			try
			{
				string	line	= process->getChildLine ( );
				cout << line << endl;
			}
			catch (...)
			{
				done	= true;
			}
		}	// while (false == done)
*/
		string	error	= "";
		error	= process->getErrorMessage ( );
//		int status	= process->getCompletionCode ( );	// v 2.25.1
//status	= 0;
		delete process;		process	= 0;	
		 if (0 != status)
		{
			ostrstream	strStatus;
			strStatus << status;
			string	message	= string (argv [0]) +
					+ ". Erreur inattendue lors de la terminaison de "
					+ argv [processArg] + " : " + error
			        + " Status de terminaison : " 
					+ string (strStatus.str ( ));
			throw Exception (message);
		}

		return status;
	}
	catch (const Exception& exc)
	{
		cout << "Exception caught : " << exc.getFullMessage ( ) << endl;
		return -1;
	}
	catch (const exception& e)
	{
		cout << "Standard exception caught : " << e.what ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cout << "Undocumented exception caught." << endl;
		return -1;
	}

	return 0;
}	// main
