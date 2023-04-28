/**
 * Client dont les fonctions sont de :
 * - Lancer le service de nommage sur une machine
 * - Lancer le serveur sur une machine
 * - Se connecter au serveur
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

static string	srvHostName;
static string	nsHostName;
static string	srvExe;
static string	parallelLauncher;
static string	oceaneIOR;
static string	tcpIpAddr;


int main (int argc, char* argv [], char* envp [])
{
	if ((5 >= argc) || (0 != getServerName (argc, argv)) ||
	    (0 != getServerPath (argc, argv)) || (0 != getNSName (argc, argv)))
	{
		cout << "Syntax : " << argv [0]
		     << " -host machine -server path [-nshost machine][-NO_NS]"
		     << "[-parallelLauncher launcher]"
		     << endl
		     << "-host ................ : machine où lancer le serveur" << endl
		     << "-server .............. : exécutable du serveur" << endl
		     << "-nshost .............. : machine où lancer le service "
		     << "de nommage" << endl
		     << "-NO_NS ............... : inutilisation du service de "
		     << "nommage (connection directe client/serveur)." << endl
		     << "-parallelLauncher .... : éventuel lanceur sur partition"
			 << " parallèle type \"mprun\"" << endl
	         << endl;
		return -1;
	}
	getParallelLauncher (argc, argv);

	// Le service de nommage :
	string	nsPort		= "2809";
	string	nsDir		= "/tmp/NS";
	string	nsExe		= "/bin/NS";
	unique_ptr<RemoteProcess>	ns;
	// Le serveur :
	unique_ptr<RemoteProcess>	srv;

	int	error	= 0;
	try
	{
		UserData		user;
		// Initialisation de l'ORB :
		cout << "ORB initialization ..." << endl;
		Process::initialize (argc, argv, envp);
		ORBFacilities::initialize (argc, argv);

		// Le lanceur de process sur le réseau :
		RemoteProcess::remoteShell	= "ssh";
		vector<string>	remoteShellOptions;
		remoteShellOptions.push_back ("-l");
		remoteShellOptions.push_back (user.getName ( ));
		UTF8String	tunnelOpt (charset);
		tunnelOpt << "-R" << nsPort << ":" << srvHostName << ":" << nsPort;
		remoteShellOptions.push_back (tunnelOpt);
		RemoteProcess::launcher		= "/bin/TkUtilLauncher";
		size_t	opt	= 0;

		if (0 == nsHostName.length ( ))
			cout << "ATTENTION : inutilisation du service de nommage." << endl;
		else
		{
		cout << "Lancement du service de nommage sur la machine "
		     << nsHostName << " ..." << endl;
		// Preparation du service de nommage :
		UTF8String	nsDir (charset);
		nsDir << "/tmp/" << user.getName ( ) << "/connect/NS";
		// Suppression des éventuels fichiers déjà présents :
		unique_ptr<RemoteProcess>	rmNs (new RemoteProcess (nsHostName, "rm"));
		UTF8String		file1 (nsDir), file2 (nsDir), file3 (nsDir),
						file4 (nsDir);
		rmNs->getOptions ( ).addOption ("-f");
		rmNs->getOptions ( ).addOption ("-r");
		rmNs->getOptions ( ).addOption (nsDir);
		rmNs->execute (false);
		rmNs->wait ( );
		// Création du répertoire des logs :
		unique_ptr<RemoteProcess>	mkdirNs(new RemoteProcess(nsHostName, "mkdir"));
		mkdirNs->getOptions ( ).addOption ("-p");
		mkdirNs->getOptions ( ).addOption (nsDir.iso ( ));
		cout << "Création du répertoire " << nsDir
		     << " des logs du service de nommage ..." << endl;
		mkdirNs->execute (false);
		mkdirNs->wait ( );

		// Lancement du service de nommage :
		NetworkData nsHost  =
		        NetworkData::getMachineInfos (getGivenHostName (nsHostName));
//		ns.reset (new RemoteProcess (nsHostName, nsExe, remoteShellOptions));
		ns.reset (new RemoteProcess (nsHostName, nsExe));
		ns->getOptions ( ).addOption ("-start");
		ns->getOptions ( ).addOption (nsPort);
		ns->getOptions ( ).addOption ("-logdir");
		ns->getOptions ( ).addOption (nsDir);
//		ns->getOptions ( ).addOption ("-ORBInitRef");
//		UTF8String  nsRef;
//		nsRef << "NameService=corbaname::" << getGivenHostName (nsHostName);
//		ns->getOptions ( ).addOption (nsRef);
		// Il faut utiliser une adresse TCP/IP du serveur visible depuis cette
		// station de travail :
		UTF8String	nsEndPoint (charset);
		nsEndPoint << "giop:tcp:"
		         << NetworkData::getUsableInetAddress (nsHostName) << ":";
		ns->getOptions ( ).addOption ("-ORBendPoint");
		ns->getOptions ( ).addOption (nsEndPoint);
//ns->getOptions ( ).addOption ("-ORBacceptBiDirectionalGIOP");
//ns->getOptions ( ).addOption ("1");
		ns->setServerWaitingMessage ("Checkpointing completed.");
		ns->execute (false);
		ns->enableChildToSonCommunications (false);
		while (true)
		{
			string  line    = ns->getChildLine ( );
			if (0 == line.length ( ))
				break;
			cout << "NS : " << line << endl;
		}	// while (true)
		
		// Connexion au service de nommage :
		cout << "Connection au service de nommage ..." << endl;
		ORBFacilities::resolveCosNaming (
						nsHostName, NumericConversions::strToULong (nsPort));
		// Rem : sur calculateur il arrive que cette connection échoue (parfois,
		// assez fréquent). Est-ce du au port (2809) qui n'est pas encore libéré
		// de la connection précédente ??? Le même produit de compilation, à
		// quelques minutes d'intervalle, passe de "ne marche pas" à
		// fonctionne ... A priori non, car après de nombreuses minutes sans
		// rien, la première tentative échoue, et trois autres juste après
		// fonctionnent ...

		}	// else if (0 == nsHostName.length ( ))

		// Lancement du serveur :
		remoteShellOptions.clear ( );
		remoteShellOptions.push_back ("-l");
		remoteShellOptions.push_back (user.getName ( ));
// Le tunneling ne fonctionne pas, le client semble tenter d'entrer
// directement en communication avec le serveur.
UTF8String	tunnelingOpt (charset);
tunnelingOpt << "3000:machine50.nom_domaine:3000";
remoteShellOptions.push_back ("-L");
remoteShellOptions.push_back (tunnelingOpt);
		cout << "Lancement du serveur " << srvExe << " sur la machine "
		     << srvHostName << " ..." << endl;
		if (0 != parallelLauncher.length ( ))
		{
			cout << "Utilisation du lanceur sur partition parallèle "
			     << parallelLauncher << endl;
			srv.reset (new RemoteProcess (
							srvHostName, parallelLauncher, remoteShellOptions));
			srv->getOptions ( ).addOption (srvExe);
		}	// if (0 != parallelLauncher.length ( ))
		else
		{
			srv.reset (
				new RemoteProcess (srvHostName, srvExe, remoteShellOptions));
		}	// else if (0 != parallelLauncher.length ( ))
		if (0 == nsHostName.length ( ))
			srv->getOptions ( ).addOption ("-NO_NS");
		else
		{
			srv->getOptions ( ).addOption ("-NS_HOST");
			srv->getOptions ( ).addOption (nsHostName);
			srv->getOptions ( ).addOption ("-NS_PORT");
			srv->getOptions ( ).addOption (nsPort);
		}	// else if (0 == nsHostName.length ( ))
		// Il faut utiliser une adresse TCP/IP du serveur visible depuis cette
		// station de travail :
		if (0 == parallelLauncher.length ( ))
		{
			UTF8String	endPoint (charset);
			endPoint << "giop:tcp:"
			         << NetworkData::getUsableInetAddress (srvHostName) << ":";
			srv->getOptions ( ).addOption ("-ORBendPoint");
			srv->getOptions ( ).addOption (endPoint);
		}	// if (0 == parallelLauncher.length ( ))
//srv->getOptions ( ).addOption ("-ORBacceptBiDirectionalGIOP");
//srv->getOptions ( ).addOption ("1");
		srv->setServerWaitingMessage ("OCEANE is waiting for requests ...");
		srv->execute (false);
		srv->enableChildToSonCommunications (false);
		string  lines;
		while (true)
		{
			string  line    = srv->getChildLine ( );
			if (0 == line.length ( ))
				break;
			cout << "SRV : " << line << endl;
			getServerEndPoint (line);
			getOceaneIOR (line);
		}	// while (true)

		// Avoir un proxy sur Océane :
		cout << "Server TCP/IP Addr : " << tcpIpAddr << endl;
		cout << "Trying to perform a connection to OCEANE ..." << endl;
		CORBA::Object_var	obj;
		if (0 == nsHostName.length ( ))
		{
			cout << "Using Oceane IOR " << oceaneIOR << " ..." << endl;
			obj	= ORBFacilities::getORB ( )->string_to_object (
														oceaneIOR.c_str ( ));
		}
		else
		{

			cout << "Using name service facilities ..." << endl;
			const string	oceaneId ("serveur"), oceaneKind ("Oceane");
			obj	= ORBFacilities::getObjectReference (oceaneId, oceaneKind);

		}	// else if (0 == nsHostName.length ( ))
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
		if (0 != srv.get ( ))
			srv->kill ( );
		ORBFacilities::finalize ( );
		if (0 != ns.get ( ))
			ns->kill ( );
	}
	catch (const IN_UTIL Exception& ue)
	{
		cout << "Exception caught : " << ue.getFullMessage ( ) << endl;
		error	= -1;
	}
	catch (const OceaneExceptionICorba& exc)
	{
cout << __FILE__ << ' ' << __LINE__ << endl;
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
		string  lines;
		while (true)
		{
			string  line    = srv->getChildLine ( );
			if (0 == line.length ( ))
				break;
			cout << "SRV : " << line << endl;
		}	// while (true)
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
		if (0 != srv.get ( ))
		{
cout << "Killing server at " << srv->getHostName ( )
     << " PID = " << srv->getRemotePid ( )
	 << " Local PID = " << srv->getPid ( ) << endl;
			srv->kill ( );
		}
	}
	catch (...)
	{ }
	try
	{
		if (0 != ns.get ( ))
		{
cout << "Killing name service at " << ns->getHostName ( )
     << " PID = " << ns->getRemotePid ( )
	 << " Local PID = " << ns->getPid ( ) << endl;
			ns->kill ( );
		}
	}
	catch (...)
	{ }

	return error;
}	// main


static int getServerName (int argc, char* argv [])
{
	for (size_t i = 0; i < argc - 1; i++)
		if (0 == strcmp ("-host", argv [i]))
		{
			srvHostName	= argv [i + 1];
			return 0;
		}

	return -1;
}	// getServerName


static int getServerPath (int argc, char* argv [])
{
	for (size_t i = 0; i < argc - 1; i++)
		if (0 == strcmp ("-server", argv [i]))
		{
			srvExe	= argv [i + 1];
			return 0;
		}

	return -1;
}	// getServerPath


static int getNSName (int argc, char* argv [])
{
	for (size_t i = 0; i < argc; i++)
	{
		if (0 == strcmp ("-NO_NS", argv [i]))
		{
			nsHostName.clear ( );
			return 0;
		}
		else if ((0 == strcmp ("-nshost", argv [i])) && (i < argc - 1))
		{
			nsHostName	= argv [i + 1];
			return 0;
		}	// if ((0 == strcmp ("-nshost", argv [i]) ...
	}	// for (size_t i = 0; i < argc; i++)

	return -1;
}	// getNSName


static int getParallelLauncher (int argc, char* argv [])
{
	for (size_t i = 0; i < argc - 1; i++)
		if (0 == strcmp ("-parallelLauncher", argv [i]))
		{
			parallelLauncher	= argv [i + 1];
			return 0;
		}

	return -1;
}	// getParallelLauncher


static void getServerEndPoint (const string& line)
{
	const string		prefix ("TCP/IP ADDR : ");	
	string::size_type	pos	= line.find (prefix);
	if (string::npos != pos)
	{
		pos			+= prefix.length ( );
		tcpIpAddr	= line.substr (pos);
	}
}	// getServerEndPoint


static void getOceaneIOR (const string& line)
{
	const string		prefix ("IOR : ");	
	string::size_type	pos	= line.find (prefix);
	if (string::npos != pos)
	{
		pos	+= prefix.length ( );
		oceaneIOR	= line.substr (pos);
	}
}	// getOceaneIOR



