/**
 * Client très simple dont le seul objet est de se connecter à son serveur
 * via socket de domaine Unix et d'invoquer un service.
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


static string			unixSocket;
static string			oceaneIOR;


int main (int argc, char* argv [], char* envp [])
{
	for (int i = 1; i < argc - 1; i++)
	{
		if (string ("-unixSocket") == argv [i])
			unixSocket	= argv [i + 1];
		else if (string ("-ior") == argv [i])
			oceaneIOR	= argv [i + 1];
	}	// for (int i = 1; i < argc - 1; i++)
	if ((0 == unixSocket.length ( )) || (0 == oceaneIOR.length ( )))
	{
		cout << "Syntax : " << argv [0]
		     << " -unixSocket fichier" << endl
		     << " -ior IOR" << endl
	         << endl;
		return -1;
	}	// if ((0 == unixSocket.length ( )) || ...

	int	error	= 0;
	try
	{
		UserData		user;
		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		Process::initialize (argc, argv, envp);
		ORBFacilities::setCsPolicy (ORBFacilities::CORBA_CS_BSD_DIRECT);
		ORBFacilities::initialize (argc, argv);

		// Avoir un proxy sur Océane :
		cout << "Trying to perform a connection to OCEANE at "
		     << unixSocket << " with IOR " << oceaneIOR << " ..." << endl;
		CORBA::Object_var	obj	= ORBFacilities::getORB ( )->string_to_object (
															oceaneIOR.c_str( ));
		cout << "Narrowing object reference ..." << endl;
		OceaneServerICorba_var	oceane	= OceaneServerICorba::_narrow (obj);
		if (true == CORBA::is_nil (oceane))
			throw IN_UTIL Exception ("L'objet n'est pas une instance de OceaneServer");
		CORBA::ULong		id		= oceane->getId ( );
		CORBA::String_var	srvName	= oceane->getOceaneServerName ( );
		cout << "Connected to OCEANE server called "
		     << srvName << " with id " << id << "." << endl;

		oceane	= OceaneServerICorba::_nil ( );

		// Arret de l'application :
sleep (60);
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

	return error;
}	// main


