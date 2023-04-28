//
// Emulation d'un serveur CORBA fonctionnant sans service de nommage.
//

#include "CorbaUtil/ORBFacilities.h"
#include "CorbaUtil/ORBServices.h"
#include "OceaneServer_impl.h"
#include <TkUtil/InformationLog.h>
#include <TkUtil/NetworkData.h>
#include <TkUtil/Process.h>
#include <TkUtil/NumericConversions.h>

#include <TkUtil/UserData.h>

#include <stdlib.h>
#include <string>
#include <iostream>


using namespace std;
using namespace CORBA;
using namespace Oceane;
using namespace TkUtil;
using namespace CorbaFacilities;

static const Charset	charset ("àéèùô");


static int parseArgs (int argc, char* argv []);
static int syntax (const string& pgm);

static string	unixSocket;
//static string	hostname (NetworkData::getCurrentHostName ( ));
//static string	hostname (getenv ("HOSTNAME"));
static string	hostname ("");


/* extern "C" int gethostname(char *name, size_t len)
{
cout << "++++++++++++++++++++++ CP HOSTNAME !!!!!!!!! : " << hostname << endl;
	size_t	n	= hostname.length ( ) < len ? hostname.length ( ) : len;
    strncpy (name, hostname.c_str ( ), n);
    return 0;
}   // gethostname */



int main (int argc, char* argv [], char* envp [])
{
cout << __FILE__ << ' ' << __LINE__ << endl;
	if (0 != parseArgs (argc, argv))
		return syntax (argv [0]);
cout << __FILE__ << ' ' << __LINE__ << endl;

	Process::initialize (argc, argv, envp);
cout << __FILE__ << ' ' << __LINE__ << endl;

	Process*	removeProcess	= new Process ("rm");
	removeProcess->getOptions ( ).addOption ("-f");
	removeProcess->getOptions ( ).addOption (unixSocket);
	removeProcess->execute (true);
	removeProcess	= 0;
cout << __FILE__ << ' ' << __LINE__ << endl;

	NetworkData	machine;
	vector<string>	inetAdresses	= machine.getInetAddr ( );
	cout << "OCEANE V5.0.1 initialization on host "
	     << machine.getHostName ( ) << "(" << machine.getFullHostName ( )
	     << ", " << machine.getInetAddress ( ) << ")" << " ..." << endl;
	cout << "TCP/IP ADDR : " << inetAdresses[inetAdresses.size( )-1] << endl;
cout << __FILE__ << ' ' << __LINE__ << endl;

	try
	{
//		int		srvArgc	= argc + 2;
		int		srvArgc	= argc;
		char**	srvArgv	= (char**)malloc (srvArgc * sizeof (char*));
		size_t	i	= 0;
		for (i = 0; i < argc; i++)
			srvArgv [i]	= strdup (argv [i]);
//		srvArgv [i++]	= strdup ("-ORBserverTransportRule");
//		srvArgv [i++]	= strdup ("* unix");

		for (i = 0; i < srvArgc; i++)
			cout << srvArgv [i] << ' ';
		cout << endl;

		// Initialisation de l'ORB :
		cout << "Initialisation de l'ORB ..." << endl;
		ORBFacilities::setCsPolicy (ORBFacilities::CORBA_CS_DIRECT);
		ORBFacilities::initialize (srvArgc, srvArgv);
		// initialize effectué
		// => Utilisation possible de ORBFacilities::getLogDispatcher ( )
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
					InformationLog (UTF8String ("ORB initialisé.", charset)));
		// Ajout intercepteur qui remplace dans les IORs générées le nom de la
		// machine courante par celui de la machine du client :
cout << endl << endl << "====================================================================" << endl << endl;
		if (0 != hostname.length ( ))
		{
			cout << "Utilisation d'un profile socket de domaine Unix lors de "
			     << "l'encodage des IORs. Machine : " << hostname
			     << ", socket de domaine Unix : " << unixSocket << "." << endl;
			useUnixDomainProfile (hostname, unixSocket);
		}	// if (0 != hostname.length ( ))
cout << endl << endl << "====================================================================" << endl << endl;

		// Initialisation d'Océane :
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
				InformationLog (UTF8String ("Initialisation d'Océane ...", charset)));
		cout << "OCEANE initialization ..." << endl;
		UserData			user;
		OceaneServer_impl*	oceane	= new OceaneServer_impl ( );
		ORBFacilities::getRootPOA ( )->activate_object (oceane);
		CORBA::String_var	ior	=
				ORBFacilities::getORB ( )->object_to_string (oceane->_this ( ));
		cout << "SRV IOR : " << ior << endl;
		oceane->_remove_ref ( );

		// On renseigne sur quelle machine le serveur tourne, avec les
		// adresses TCP/IP :
		cout << machine.getHostName ( ) << " " << machine.getFullHostName ( )
		     << " ";
		for (vector<string>::iterator it = inetAdresses.begin ( );	
		     inetAdresses.end ( ) != it; it++)
			cout << *it << ' ';
		cout << endl;

		// Boucle principale de l'application :
		// cout : on est sûr que c'est écrit sur la sortie standard, ce qui est
		// nécessaire pour Lem.
		cout << "OCEANE server is ready." << endl;
		cout << "OCEANE is waiting for requests ..." << endl;
		ORBFacilities::run ( );

		// Fin de l'application :
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
				InformationLog (UTF8String ("Arrêt d'Océane ...", charset)));
		ORBFacilities::finalize ( );
		// finalize effectué => ORBFacilities::getLogDispatcher ( ) interdit
		cout << "Océane arrêté." << endl;
	}
	catch (const IN_UTIL Exception& ue)
	{
		cout << "Exception caught : " << ue.getFullMessage ( ) << endl;
		return -1;
	}
	catch (const CORBA::SystemException&)
	{
		cout << "CORBA::SystemException caught." << endl;
		return -2;
	}
	catch (const CORBA::Exception&)
	{
		cout << "CORBA::Exception caught." << endl;
		return -3;
	}
	catch (const exception& e)
	{
		cout << "STD Exception caught : " << e.what ( ) << endl;
		return -4;
	}
	catch (...)
	{
		cout << "Unexpected exception caught." << endl;
		return -5;
	}

	return 0;
}	// main


static int parseArgs (int argc, char* argv [])
{
	for (size_t i = 1; i < argc; i++)
	{
		if ((0 == strcmp (argv [i], "-unixSocket")) && (i < argc - 1))
		{
			unixSocket	= argv [++i];
			cout << "Using " << unixSocket << " for unix socket." << endl;
		}
		else if ((0 == strcmp (argv [i], "-hostname")) && (i < argc - 1))
		{
			hostname	= argv [++i];
			cout << "Forcing hostname as " << hostname << endl;
		}
		else if (0 == strncmp (argv [i], "-ORB", 4))
		{ i++; }
		else
			return -1;
	}	// for (size_t i = 1; i < argc; i++)

	if (0 == unixSocket.size ( ))
		return -1;

	return 0;
}	// parseArgs


static int syntax (const string& pgm)
{
	cout << pgm << " -unixSocket path [-port port][-ORB* arg] :" <<endl
	     << "-unixSocket path ............ : socket unix utilisé" << endl
	     << "-ORB* arg ................... : arguments omniORB, cf doc omniORB"
	     << endl
	     << endl;

	return -1;
}	// syntax

