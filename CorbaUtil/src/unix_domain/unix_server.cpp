//
// Emulation d'un serveur CORBA fonctionnant sans service de nommage à l'écoute
// avec un socket de domaine Unix.
//

#include "CorbaUtil/ORBFacilities.h"
#include "OceaneServer_impl.h"
#include <TkUtil/InformationLog.h>
#include <TkUtil/NetworkData.h>
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


static int parseArgs (int argc, char* argv []);
static int syntax (const string& pgm);

static string			unixSocket;

static const Charset	charset ("àéèùô");


int main (int argc, char* argv [])
{
	if (0 != parseArgs (argc, argv))
		return syntax (argv [0]);

	NetworkData	machine;
	vector<string>	inetAdresses	= machine.getInetAddr ( );
	cout << "OCEANE V5.0.1 initialization on host "
	     << machine.getHostName ( ) << "(" << machine.getFullHostName ( )
	     << ", " << machine.getInetAddress ( ) << ")" << " ..." << endl;
	cout << "TCP/IP ADDR : " << inetAdresses[inetAdresses.size( )-1] << endl;

	try
	{
		ORBFacilities::setUnixSocketFileName (unixSocket);
		int		srvArgc	= argc + 1;
		char**	srvArgv	= (char**)malloc (srvArgc * sizeof (char*));
		size_t	i	= 0;
		for (i = 0; i < argc; i++)
			srvArgv [i]	= strdup (argv [i]);
		srvArgv [i++]	= strdup ("-endPointNoPublish");

		for (i = 0; i < srvArgc; i++)
			cout << srvArgv [i] << ' ';
		cout << endl;

		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		ORBFacilities::setCsPolicy (ORBFacilities::CORBA_CS_BSD_DIRECT);
		ORBFacilities::initialize (srvArgc, srvArgv);
		// initialize effectué
		// => Utilisation possible de ORBFacilities::getLogDispatcher ( )
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
					InformationLog (UTF8String ("ORB initialisé.", charset)));

		// Initialisation d'Océane :
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
				InformationLog (UTF8String ("Initialisation d'Océane ...", charset)));
		cout << "OCEANE initialization ..." << endl;
		UserData			user;
		OceaneServer_impl*	oceane	= new OceaneServer_impl ( );
		ORBFacilities::getRootPOA ( )->activate_object (oceane);
		CORBA::String_var	ior	= ORBFacilities::getORB ( )->object_to_string (oceane->_this ( ));
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
			unixSocket	= argv [++i];
		else if (0 == strncmp (argv [i], "-ORB", 4))
		{ i++; }
		else
			return -1;
	}	// for (size_t i = 1; i < argc; i++)

	return 0;
}	// parseArgs


static int syntax (const string& pgm)
{
	cout << pgm << " [-port port][-ORB* arg] :" <<endl
	     << "-unixSocket fichier .................. : écoute sur le socket de "
	     << "domaine UNIX fichier dans l'attente de connections clientes."
	     << endl
	     << "-ORB* arg ................... : arguments omniORB, cf doc omniORB"
	     << endl
	     << endl;

	return -1;
}	// syntax
