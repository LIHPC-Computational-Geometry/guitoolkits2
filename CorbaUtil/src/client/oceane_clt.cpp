/**
 * LEM, l'utilisateur d'Océane du futur.
 */

#include "CorbaUtil/ORBFacilities.h"
#include "OceaneServerICorba.hh"

#include <TkUtil/UserData.h>

#include <string>
#include <iostream>


using namespace std;
using namespace CORBA;
using namespace Oceane;
using namespace CorbaFacilities;


static string findModel (int argc, char* argv []);


int main (int argc, char* argv [])
{
	const string	mdlFileName	= findModel (argc, argv);
	if (0 == mdlFileName.length ( ))
	{
		cout << "Syntax : " << argv [0] << " -file file_name" << endl;
		return -100;
	}	// if (0 == mdlFileName.length ( ))

	try
	{
		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		ORBFacilities::setCsPolicy (ORBFacilities::CORBA_CS_COS_NAMING);
		ORBFacilities::initialize (argc, argv);

		// Connexion au service de nommage :
//		ORBFacilities::resolveCosNaming ("localhost", 2000);
		ORBFacilities::resolveCosNaming ( );

		// Avoir un proxy sur Océane :
		IN_UTIL	UserData	user;
		cout << "Trying to perform a connection to OCEANE ..." << endl;
		const string	oceaneId ("serveur"), oceaneKind ("Oceane");
		CORBA::Object_var	obj	= 
					ORBFacilities::getObjectReference (oceaneId, oceaneKind);
		cout << "Narrowing object reference ..." << endl;
		OceaneServerICorba_var	oceane	= OceaneServerICorba::_narrow (obj);
		if (true == CORBA::is_nil (oceane))
			throw IN_UTIL Exception ("L'objet n'est pas une instance de OceaneServer");
		cout << "Connected to OCEANE server." << endl;

		// Rem : à ce stade on a une référence sur le serveur, mais elle n'a
		// encore pas été épprouvée, à savoir que le client n'a encore pas
		// effectué de requête. Si une requête échoue, avec une système
		// exception de type TRANSIENT, c'est peut être parce-que le serveur
		// s'est déclaré avec une adresse TCP/IP inutilisable par le client
		// (adresse utilisable par exemple que depuis le calculateur).
		// => Initialiser le serveur avec une adresse TCP/IP utilisable par le
		// client en utilisant l'argument de lancement du serveur 
		// -ORBendPoint giop:tcp:XXX:XXX:XXX:XXX:.
		// Cette adresse TCP/IP peut être obtenue avec
		// NetworkData::getUsableInetAddress.
		// Ouverture du fichier de modelisation :
		cout << "Loading model located in " << mdlFileName << " ..." << endl;
		CORBA::String_var	fileName	= mdlFileName.c_str ( );
		ModelICorba_var		model		= oceane->loadModel (fileName);
		if (true == CORBA::is_nil (model))
			 throw IN_UTIL Exception ("Erreur lors du chargement du modèle.");

		// On effectue une projection :
		Oceane::ModelICorba::SurfaceList_var	surfaceList	=
									new Oceane::ModelICorba::SurfaceList ( );
		surfaceList->length (1);
		surfaceList [0]	= "S1";
		cout << "Performing a projection on 1 surface S1 ..." << endl;
		const size_t				size		= 10;
		Oceane::ModelICorba::NodeSurfacesList_var	nodeSurfacesListVar	=
						new Oceane::ModelICorba::NodeSurfacesList (size);
		nodeSurfacesListVar->length (size);
		NodeList_var				nodeList	= new NodeList ( );
		ModelICorba::Distances_var	distances	= new ModelICorba::Distances( );
		nodeList->length (size);
		distances->length (size);
		cout << "Before projection nodes are : " << endl;
		size_t	n	= 0;
		Oceane::ModelICorba::NodeSurfaces	nodeSurfaces;
		nodeSurfaces.s_64	= 1;
		for (n = 0; n < size; n++)
		{
			nodeSurfacesListVar [n]	= nodeSurfaces;
			nodeList[n].x	= 1. / size;
			nodeList[n].y	= 0.;
			nodeList[n].z	= 1 + nodeList[n].x;
			cout << "(" << nodeList [n].x << ", " << nodeList [n].y << ", "
			     << nodeList[n].z << ") ";
			distances[n]	= 0;
		}
		cout << endl;
		model->projectNodes (
					surfaceList, nodeSurfacesListVar, nodeList, distances);
		cout << "After projection nodes are : " << endl;
		for (n = 0; n < size; n++)
		{
			cout << "(" << nodeList [n].x << ", " << nodeList [n].y << ", "
			     << nodeList[n].z << ") ";
		}	// for (n = 0; n < size; n++)
		cout << endl;
		cout << "Distances are : ";
		for (n = 0; n < size; n++)
			cout << distances [n] << ' ';
		cout << endl;

		// Fermeture du modele :
		cout << "Closing model " << mdlFileName << " ..." << endl;
		oceane->closeModel (model);
		cout << "Model " << mdlFileName << " closed." << endl;
		model	= ModelICorba::_nil ( );

		oceane	= OceaneServerICorba::_nil ( );

		// Arret de l'application :
		cout << "Naming service finalization ..." << endl;
		ORBFacilities::finalize ( );
	}
	catch (const IN_UTIL Exception& ue)
	{
		cout << "Exception caught : " << ue.getFullMessage ( ) << endl;
		return -1;
	}
	catch (const OceaneExceptionICorba& exc)
	{
cout << __FILE__ << ' ' << __LINE__ << endl;
		cout << "OceaneException caught : " << exc.what << endl;
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


static string findModel (int argc, char* argv [])
{
	for (int i = 0; i < argc - 1; i++)
	{
		if (string ("-file") == argv [i])
			return argv [i + 1];
	}	// for (int i = 0; i < argc - 1; i++)

	return "";
}	// findModel


