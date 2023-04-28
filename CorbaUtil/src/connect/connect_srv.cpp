#include "CorbaUtil/ORBFacilities.h"
#include "OceaneServer_impl.h"
#include <TkUtil/InformationLog.h>
#include <TkUtil/NetworkData.h>

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


static bool		useNS	= true;

static int parseArgs (int argc, char* argv []);
static int syntax (const string& pgm);


int main (int argc, char* argv [])
{
	if (0 != parseArgs (argc, argv))
		return syntax (argv [0]);

	if (false == useNS)
		cout << "ATTENTION : inutilisation du service de nommage." << endl;

	NetworkData	machine;
	vector<string>	inetAdresses	= machine.getInetAddr ( );
	cout << "OCEANE V5.0.1 initialization on host "
	     << machine.getHostName ( ) << "(" << machine.getFullHostName ( )
	     << ", " << machine.getInetAddress ( ) << ")" << " ..." << endl;
	cout << "TCP/IP ADDR : " << inetAdresses[inetAdresses.size( )-1] << endl;

	try
	{
		int		srvArgc	= argc + 2;
		char**	srvArgv	= (char**)malloc (srvArgc * sizeof (char*));
		size_t	i	= 0;
		for (i = 0; i < argc; i++)
			srvArgv [i]	= strdup (argv [i]);
		srvArgv [i++]	= strdup ("-ORBendPoint");
		UTF8String	endPoint (charset);
		endPoint << "giop:tcp:" << inetAdresses[inetAdresses.size( )-1] << ":";
		if (false == useNS)
			endPoint << "3000";
		srvArgv [i++]	= strdup (endPoint.iso ( ).c_str ( ));

		for (i = 0; i < srvArgc; i++)
			cout << srvArgv [i] << ' ';
		cout << endl;

		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		ORBFacilities::initialize (srvArgc, srvArgv);
		// initialize effectué
		// => Utilisation possible de ORBFacilities::getLogDispatcher ( )
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (InformationLog (UTF8String ("ORB initialisé.", charset)));

		// Connexion au service de nommage :
		if (true == useNS)
			ORBFacilities::resolveCosNaming ( );

		// Initialisation d'Océane :
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (InformationLog (UTF8String ("Initialisation d'Océane ...", charset)));
		cout << "OCEANE initialization ..." << endl;
		UserData			user;
		OceaneServer_impl*	oceane	= new OceaneServer_impl ( );
		if (true == useNS)
		{
			ORBFacilities::bindObjectToName ("serveur", "Oceane", oceane->_this ( ), true);
			oceane->_remove_ref ( );
			if (0 != ORBFacilities::getLogDispatcher ( ))
				ORBFacilities::getLogDispatcher ( )->log (InformationLog (UTF8String ("Serveur Océane référencé auprès du service de nommage.", charset)));
		}	// if (true == useNS)
		else
		{
			ORBFacilities::getRootPOA ( )->activate_object (oceane);
			CORBA::String_var	ior	=
				ORBFacilities::getORB ( )->object_to_string (oceane->_this ( ));
			cout << "SRV IOR : " << ior << endl;
			oceane->_remove_ref ( );
		}	// else if (true == useNS)

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
		if ((0 == strcmp (argv [i], "-NS_HOST")) && (i < argc - 1))
			i++;
		else if ((0 == strcmp (argv [i], "-NS_PORT")) && (i < argc - 1))
			i++;
		else if (0 == strcmp (argv [i], "-NO_NS"))
			useNS	= false;
		else if (0 == strncmp (argv [i], "-ORB", 4))
		{ i++; }
//		else
//			return -1;
	}	// for (size_t i = 1; i < argc; i++)

	return 0;
}	// parseArgs


static int syntax (const string& pgm)
{
	cout << pgm << " [-NS_HOST host][-NS_PORT port][-NO_NS][-ORB* arg] :" <<endl
	     << "-NS_HOST host .............. : connection au service de nommage "
	     << "de la machine host" << endl
	     << "-NS_PORT port ............... : connection au port port pour le "
	     << "service de nommage" << endl
	     << "-NO_NS ...................... : absence de référencement auprès "
	     << "du service de nommage" << endl
	     << "-ORB* arg ................... : arguments omniORB, cf doc omniORB"
	     << endl
	     << endl;

	return -1;
}	// syntax
