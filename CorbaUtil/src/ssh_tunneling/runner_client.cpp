/**
 * Client lançant le serveur sur une machine donnée, attendant son lancement
 * effectif, récupérant son IOR, et s'y connectant via tunneling ssh.
 */

#include "CorbaUtil/ORBFacilities.h"
#include "CorbaUtil/ORBServices.h"
#include "OceaneServerICorba.hh"

#include <TkUtil/NetworkData.h>
#include <TkUtil/NumericConversions.h>
#include <TkUtil/RemoteProcess.h>
#include <TkUtil/UserData.h>

#include <string>
#include <iostream>
#include <memory>


using namespace std;
using namespace TkUtil;
using namespace CORBA;
using namespace Oceane;
using namespace CorbaFacilities;

static const Charset	charset ("àéèùô");


static int getServerName (int argc, char* argv []);
static int getServerPath (int argc, char* argv []);
static int getNSName (int argc, char* argv []);
static int getParallelLauncher (int argc, char* argv []);
static void getServerEndPoint (const string& line);
static void getOceaneIOR (const string& line);

static string			host (NetworkData::getCurrentHostName ( ));
static string			hostname (NetworkData::getCurrentHostName ( ));

static string			oceaneIOR;
static string			unixSocket ("/tmp/cu.socket.12345");
static string			srvPath;
static string			sshPath ("/usr/bin/ssh");
static vector<string>	sshOptions;


int main (int argc, char* argv [], char* envp [])
{
	Process::initialize (argc, argv, envp);

	for (int i = 1; i < argc; i++)
	{
		if ((0 == strcmp ("-host", argv [i])) && (i < argc - 1))
			host	= argv [++i];
		else if ((0 == strcmp ("-srvPath", argv [i])) && (i < argc - 1))
			srvPath	= argv [++i];
		else if ((0 == strcmp ("-unixSocket", argv [i])) && (i < argc - 1))
			unixSocket	= argv [++i];
		else if ((0 == strcmp ("-sshPath", argv [i])) && (i < argc - 1))
			sshPath	= argv [++i];
		else if ((0 == strcmp ("-sshOpt", argv [i])) && (i < argc - 1))
			sshOptions.push_back (argv [++i]);
	}	// for (int i = 1; i < argc; i++)

	Process*	removeProcess	= new Process ("rm");
	removeProcess->getOptions ( ).addOption ("-f");
	removeProcess->getOptions ( ).addOption (unixSocket);
	removeProcess->execute (true);
	removeProcess	= 0;

	RemoteProcess::launcher	= "TkUtilLauncher";
	RemoteProcess::remoteShell	= sshPath;

	if (0 == srvPath.length ( ))
	{
		cout << "Syntax : " << argv [0]
		     << " -srvPath path" << endl
		     << " [-host machine]" << endl
		     << " [-unixSocket path]" << endl
		     << " [-sshPath path]" << endl
		     << " [-sshOpt opt1][-sshOpt opt2]...[-sshOpt optn]" << endl
	         << endl;
		return -1;
	}	// if (0 == srvPath.length ( ))

// Lancement du serveur :
	UTF8String	endPoint (charset);
	endPoint << "giop:unix:" << unixSocket;
//	sshOptions.push_back ("-L");
//	sshOptions.push_back ("/tmp/lem.socket.12345:/tmp/lem.socket.12345");
	unique_ptr<RemoteProcess>	server (new RemoteProcess (host, srvPath, sshOptions));
// -ORBendPointNoPublish giop:unix:: : non car la IOR ne contiendra plus le
// nom du fichier socket unix.
	server->getOptions ( ).addOption ("-hostname");
	server->getOptions ( ).addOption (hostname);
	server->getOptions ( ).addOption ("-unixSocket");
	server->getOptions ( ).addOption (unixSocket);
//	server->getOptions ( ).addOption ("-ORBendPointNoPublish");
//	server->getOptions ( ).addOption ("giop:unix:");
	server->getOptions ( ).addOption ("-ORBendPoint");
	server->getOptions ( ).addOption (endPoint);
server->getOptions ( ).addOption ("-ORBtraceLevel");
server->getOptions ( ).addOption ("40");
	server->setServerWaitingMessage ("OCEANE is waiting for requests ...");
cout << "Running server with following command line :" << endl
     << server->getCommandLine ( ) << endl;

	int	error	= 0;
	try
	{
		// On lance le serveur :
		server->execute (false);
		const pid_t	pid	= server->getRemotePid ( );
cout << "Server PID on host " << host << " is " << (unsigned long)pid << endl;
		// Récupération de la IOR du serveur via son stdout :
		server->enableChildToSonCommunications (false);

		while (true)
		{
			const string	iorDeclaration ("SRV IOR : ");
			string	line	= server->getChildLine ( );
			if (0 != line.size ( ))
				cout << "SRV :" << line << endl;
			if (0 == line.find (iorDeclaration))
			{
				oceaneIOR	=
					UTF8String (line).substring (iorDeclaration.length ( ));
				break;
			}	// if (0 == line.find (iorDeclaration))
		}	// while (true)

		UserData		user;
		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		ORBFacilities::setCsPolicy (ORBFacilities::CORBA_CS_DIRECT);
		ORBFacilities::initialize (argc, argv);
		omniORB::setClientCallTimeout (0);

cout << "IOR got is : " << oceaneIOR << endl;
//cout << "IOR got is : " << oceaneIOR << ". Migrating it on host "
//     << NetworkData::getCurrentHostName ( ) << " ..." << endl;
//CorbaFacilities::migrateIOR (oceaneIOR, NetworkData::getCurrentHostName ( ));
//cout << " IOR is now " << oceaneIOR << "." << endl;

		// Avoir un proxy sur Océane :
		cout << "Trying to perform a connection to OCEANE with IOR "
		     << oceaneIOR << "..." << endl;
		CORBA::Object_var		obj;
		OceaneServerICorba_var	oceane;
		cout << "Using Oceane IOR " << oceaneIOR << " ..." << endl;
		obj	= ORBFacilities::getORB ( )->string_to_object (oceaneIOR.c_str ( ));
		cout << "Narrowing object reference ..." << endl;
		oceane	= OceaneServerICorba::_narrow (obj);
		cout << "Narrowed." << endl;
		if (true == CORBA::is_nil (oceane))
			throw IN_UTIL Exception (
							"L'objet n'est pas une instance de OceaneServer");
		CORBA::ULong		id		= oceane->getId ( );
		CORBA::String_var	srvName	= oceane->getOceaneServerName ( );
		cout << "Connected to OCEANE server called "
		     << srvName << " with id " << id << "." << endl;

		oceane	= OceaneServerICorba::_nil ( );

		// Arret de l'application :
//sleep (60);
		cout << "Killing server ..." << endl;
		server->kill ( );
		cout << "Server killed." << endl;
		cout << "Application finalization ..." << endl;
		ORBFacilities::finalize ( );
	}
	catch (const IN_UTIL Exception& ue)
	{
		cout << "Exception caught : " << ue.getFullMessage ( ) << endl;
		error	= -1;
	}
	catch (const OceaneExceptionICorba& exc)
	{
		cout << "OceaneException caught : " << exc.what << endl;
		error	= -6;
	}
	catch (const CORBA::SystemException& cse)
	{
#ifndef BullEL_6
#	ifndef RHEL_6
#		ifndef Atos_7
		cout << "CORBA::SystemException caught. Minor = " << cse.minor ( )
		     << " Name = " << cse._name ( ) << endl;
#		endif	// Atos_7
#	endif	// RHEL_6
#endif	// BullEL_6
		cout << "CORBA::SystemException caught. "
		     << " Name = " << cse._name ( ) << endl;
		error	= -2;
	}
	catch (const CORBA::Exception&)
	{
		cout << "CORBA::Exception caught." << endl;
		error	= -3;
	}
	catch (const exception& e)
	{
		cout << "STD Exception caught : " << e.what ( ) << endl;
		error	= -4;
	}
	catch (...)
	{
		cout << "Unexpected exception caught." << endl;
		error	= -5;
	}

	try
	{
		cout << "Killing server ..." << endl;
		server->kill ( );
	}
	catch (...)
	{
	}

	return error;
}	// main


