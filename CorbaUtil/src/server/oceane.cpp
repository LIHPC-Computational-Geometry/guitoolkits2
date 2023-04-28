#include "CorbaUtil/ORBFacilities.h"
#include "OceaneServer_impl.h"
#include <TkUtil/InformationLog.h>

#include <TkUtil/UserData.h>

#include <string>
#include <iostream>


using namespace std;
using namespace CORBA;
using namespace Oceane;
using namespace TkUtil;
using namespace CorbaFacilities;

static const Charset	charset ("àéèùô");


int main (int argc, char* argv [])
{
	cout << "OCEANE V5.0.1 initialization ..." << endl;

	try
	{
		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		ORBFacilities::setCsPolicy (ORBFacilities::CORBA_CS_COS_NAMING);
		ORBFacilities::initialize (argc, argv);
		// initialize effectué
		// => Utilisation possible de ORBFacilities::getLogDispatcher ( )
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
					InformationLog (UTF8String ("ORB initialisé.", charset)));

		// Connexion au service de nommage :
		ORBFacilities::resolveCosNaming ( );

		// Initialisation d'Océane :
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
				InformationLog (UTF8String ("Initialisation d'Océane ...", charset)));
		cout << "OCEANE initialization ..." << endl;
		UserData			user;
		OceaneServer_impl*	oceane	= new OceaneServer_impl ( );
		ORBFacilities::bindObjectToName (
					"serveur", "Oceane", oceane->_this ( ), true);
		oceane->_remove_ref ( );
		if (0 != ORBFacilities::getLogDispatcher ( ))
			ORBFacilities::getLogDispatcher ( )->log (
					InformationLog (UTF8String ("Serveur Océane référencé auprès du service de nommage.", charset)));

		// Boucle principale de l'application :
		// cout : on est sûr que c'est écrit sur la sortie standard, ce qui est
		// nécessaire pour Lem.
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
