#include "CorbaUtil/ORBFacilities.h"
#include "CorbaUtil/ORBServices.h"
#include <CorbaUtil/NamingFacilities.h>
#include <TkUtil/NumericConversions.h>
#include <TkUtil/Exception.h>
#include <TkUtil/File.h>
#include <TkUtil/UTF8String.h>
#include <TkUtil/NetworkData.h>
#include <TkUtil/MemoryError.h>
#include <TkUtil/DefaultLogFactory.h>
#include <TkUtil/InformationLog.h>
#include <TkUtil/ErrorLog.h>
#include <TkUtil/TraceLog.h>


#include <stdlib.h>
#include <strings.h>

#include <iostream>

using namespace std;
using namespace CORBA;
USING_UTIL


static const Charset	charset ("àéèùô");


namespace CorbaFacilities
{

ORB_var							ORBFacilities::_orb;
PortableServer::POA_var			ORBFacilities::_rootPOA;
bool							ORBFacilities::_enableBidirPOA	= false;
PortableServer::POA_var			ORBFacilities::_bidirPOA;
PortableServer::POAManager_var	ORBFacilities::_poaManager;
ORBFacilities::CORBA_CS_POLICY	ORBFacilities::_csPolicy	=
											ORBFacilities::CORBA_CS_DIRECT;
bool							ORBFacilities::_useTunneling	= true;
string							ORBFacilities::_nsHostName		= 
									IN_UTIL NetworkData::getCurrentHostName ( );
bool							ORBFacilities::_isDefaultNSHost	= true;
size_t							ORBFacilities::_nsPort			= 2809;
bool							ORBFacilities::_isDefaultNSPort	= true;
string							ORBFacilities::_unixSocketFileName;
#ifdef OMNIORB
// 2097152	: Valeur par défaut de omniORB
size_t							ORBFacilities::_giopMaxMsgSize	= 2097152;
#else
#error	"ORB non défini"
#endif	// OMNIORB
LogDispatcher*					ORBFacilities::_logDispatcher	= 0;


void ORBFacilities::initialize (int argc, char* argv [])
{
	ORBFacilities::initialize (argc, argv, getCsPolicy ( ));
}	// ORBFacilities::initialize


void ORBFacilities::initialize (int argc, char* argv [], CORBA_CS_POLICY policy)	// v 5.4.0
{
	if ((false == is_nil (_orb)) || (0 != _logDispatcher))
		throw IN_UTIL Exception (
				UTF8String ("ORBFacilities::initialize : ORB déjà initialisé.", charset));
	_logDispatcher			= new LogDispatcher ( );
	CHECK_NULL_PTR_ERROR (_logDispatcher)

	BEGIN_CORBA_TRY_CATCH_BLOCK

	parseArgs (argc, argv);
	log (InformationLog (UTF8String ("ORBFacilities::initialize : Initialisation des services CORBA ...", charset)));

	// ATTENTION : ORB_init peut faire des desallocations et decrementer argc.
	int		orbArgc	= argc, additionalArgs	= 0;
	if ((CORBA_CS_BSD_DIRECT == policy) && (true == useSSHTunneling ( )))
		additionalArgs	+= 2;
//additionalArgs	+= 2;	// ORBtraceLevel
	char**	orbArgv	= (char**)malloc ((orbArgc + additionalArgs) * sizeof (char*));
	int		o		= 0;
	for (o = 0; o < orbArgc; o++)
		orbArgv [o]	= strdup (argv [o]);
	if ((ORBFacilities::CORBA_CS_BSD_DIRECT == policy) && (true == useSSHTunneling ( )))
	{
//		orbArgv [o++]	= strdup ("-endPointNoPublish");
		orbArgv [o++]	= strdup ("-ORBendPoint");
		UTF8String		bsdOpt (charset);
		bsdOpt << "giop:unix:" << getUnixSocketFileName (true);
		orbArgv [o++]	= strdup (bsdOpt.iso ( ).c_str ( ));
	}	// if ((ORBFacilities::CORBA_CS_BSD_DIRECT == getCsPolicy ( )) && ...
//orbArgv [o++]	=  strdup ("-ORBtraceLevel");
//orbArgv [o++]	=  strdup ("40");
	int	argCount	= orbArgc + additionalArgs;
//cout << endl << endl << endl << "CALLING ORB_INIT WITH :" << endl;
//for (int j = 0; j < argCount; j++)
//	cout << orbArgv [j] << ' ';
//cout << endl << endl << endl;
	_orb	= CORBA::ORB_init (argCount, orbArgv);
	if (true == is_nil (_orb))
		throw IN_UTIL Exception (UTF8String (
				"ORBFacilities::initialize : impossible d'initialiser l'ORB.", charset));

	CORBA::Object_var	obj	= _orb->resolve_initial_references ("RootPOA");
	_rootPOA	= PortableServer::POA::_narrow (obj);
	if (true == is_nil (_rootPOA))
		throw IN_UTIL Exception (UTF8String (
				"ORBFacilities::initialize : impossible d'obtenir une référence sur le POA racine.", charset));
	_poaManager	= _rootPOA->the_POAManager ( );
	if (true == is_nil (_poaManager))
		throw IN_UTIL Exception (UTF8String (
				"ORBFacilities::initialize : impossible d'obtenir une référence sur le manager de POAs.", charset));

	ORB_CATCH_BLOCK
	COMPLETE_CORBA_TRY_CATCH_BLOCK

	if (true == hasError)
	{
		IN_UTIL UTF8String	error (charset);
		error << "Impossibilité d'initialiser l'ORB : " << errorMsg << ".";
		log (ErrorLog (error));
		throw IN_UTIL Exception (error);
	}	// if (true == hasError)
	else
		log (InformationLog (UTF8String ("ORBFacilities::initialize : services CORBA initialisés avec succès.", charset)));
}	// ORBFacilities::initialize


void ORBFacilities::run ( )
{
	if (true == is_nil (_orb))
		throw IN_UTIL Exception (UTF8String ("ORBFacilities::run : ORB non initialisé.", charset));
	if (true == is_nil (_rootPOA))
		throw IN_UTIL Exception (UTF8String ("ORBFacilities::run : Absence de POA.", charset));
	
	log (InformationLog (UTF8String ("ORBFacilities::run : lancement de la boucle évènementielle CORBA ...", charset)));
	_poaManager->activate ( );
	TRACE_LOG_5 (trace, UTF8String ("ORBFacilities::run : POA manager activé.", charset))
	log (trace);
	_orb->run ( );
	log (InformationLog (UTF8String ("ORBFacilities::run : boucle évènementielle CORBA terminée.", charset)));
}	// ORBFacilities::run


void ORBFacilities::finalize ( )
{
	NamingFacilities::finalize ( );
	_rootPOA	= 0;
	_orb		= 0;
	if (0 != getLogDispatcher ( ))
		log (InformationLog (UTF8String (
				"ORBFacilities::finalize : Fin des services CORBA", charset)));

	delete _logDispatcher;
	_logDispatcher			= 0;
}	// ORBFacilities::finalize


ORB_ptr ORBFacilities::getORB ( )
{
	return _orb;
}	// ORBFacilities::getORB


PortableServer::POA_var& ORBFacilities::getBidirPOA ( )
{
	if (false == CORBA::is_nil (_bidirPOA))
		return _bidirPOA;

	CORBA::PolicyList	policyList;
	policyList.length (1);
	CORBA::Any			any;
	any <<= BiDirPolicy::BOTH;
	policyList [0]	= getORB ( )->create_policy (
							BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, any);
	_bidirPOA	= _rootPOA->create_POA ("bidir", _poaManager, policyList);

	return _bidirPOA;
}	// ORBFacilities::getBidirPOA


PortableServer::POA_var& ORBFacilities::getUsedPOA ( )
{
	return true == _enableBidirPOA ? getRootPOA ( ) : getBidirPOA ( );
}	// ORBFacilities::getUsedPOA


void ORBFacilities::setCsPolicy (CORBA_CS_POLICY policy)
{
	switch (policy)
	{
		case ORBFacilities::CORBA_CS_COS_NAMING		:
		break;
		case ORBFacilities::CORBA_CS_DIRECT			:
		case ORBFacilities::CORBA_CS_BSD_DIRECT		:
		case ORBFacilities::CORBA_CS_REVERSE		:
			if (true == isNameServiceInited ( ))
			{
				UTF8String	message (charset);
				message << "ORBFacilities::setCsPolicy. Affectation du mode "
				        << "de fonctionnement client/serveur "
				        << ORBFacilities::csPolicyToString (policy)
				        << " alors que le service de nommage est en "
				       << "fonctionnement.";
				throw IN_UTIL Exception (message);
			}
		break;
	}	// ORBFacilities::setCsPolicy

	_csPolicy	= policy;
}	// ORBFacilities::setCsPolicy


void ORBFacilities::setSSHTunneling (bool on)
{
	if (on == _useTunneling)
		return;

	_useTunneling	= on;
}	// ORBFacilities::setSSHTunneling


string ORBFacilities::csPolicyToString (ORBFacilities::CORBA_CS_POLICY policy)
{
	switch (policy)
	{
		case ORBFacilities::CORBA_CS_COS_NAMING	: return "cosNaming";
		case ORBFacilities::CORBA_CS_DIRECT		: return "direct";
		case ORBFacilities::CORBA_CS_BSD_DIRECT	: return "directBSD";
		case ORBFacilities::CORBA_CS_REVERSE	: return "reverse";
	}	// switch (policy)

	return "inconnu";
}	// ORBFacilities::csPolicyToString


ORBFacilities::CORBA_CS_POLICY ORBFacilities::stringToCsPolicy (const string& p)
{
	if (p == "cosNaming")
		return ORBFacilities::CORBA_CS_COS_NAMING;
	else if (p == "direct")
		return ORBFacilities::CORBA_CS_DIRECT;
	else if (p == "directBSD")
		return ORBFacilities::CORBA_CS_BSD_DIRECT;
	else if (p == "reverse")
		return ORBFacilities::CORBA_CS_REVERSE;

	UTF8String	message (charset);
	message << "ORBFacilities::stringToCsPolicy, mode de fonctionnement "
	        << "client/serveur inconnu : " << p << ".";
	throw IN_UTIL Exception (message);
}	// ORBFacilities::stringToCsPolicy


bool ORBFacilities::isNameServiceInited ( )
{
	return NamingFacilities::isInited ( );
}	// ORBFacilities::isNameServiceInited


const string& ORBFacilities::getNSHostName ( )
{
	return _nsHostName;
}	// ORBFacilities::getNSHostName


bool ORBFacilities::isDefaultNSHost ( )
{
	return _isDefaultNSHost;
}	// ORBFacilities::isDefaultNSHost


size_t ORBFacilities::getNSPort ( )
{
	return _nsPort;
}	// ORBFacilities::getNSPort


bool ORBFacilities::isDefaultNSPort ( )
{
	return _isDefaultNSPort;
}	// ORBFacilities::isDefaultNSPort


string ORBFacilities::getUnixSocketFileName (bool createIfNecessary)
{
	if (true == _unixSocketFileName.empty ( ))
	{
		_unixSocketFileName	= File::createTemporaryName ("corba_util", true);
	}	// if (true == _unixSocketFileName.empty ( ))

	return _unixSocketFileName;
}	// ORBFacilities::getUnixSocketFileName


void ORBFacilities::setUnixSocketFileName (const string& fileName)
{
	_unixSocketFileName	= fileName;
}	// ORBFacilities::setUnixSocketFileName


size_t ORBFacilities::getGiopMaxMsgSize ( )
{
	return _giopMaxMsgSize;
}	// ORBFacilities::getGiopMaxMsgSize


void ORBFacilities::resolveCosNaming (string host, size_t port)
{
	try
	{
		UTF8String	info1 (charset), info2 (charset);
		if (host != getNSHostName ( ))
		{
			_nsHostName			= host;
			_isDefaultNSHost	= false;
		}
		if (port != getNSPort ( ))
		{
			_nsPort				= port;
			_isDefaultNSPort	= false;
		}
		info1 << "ORBFacilities::resolveCosNaming. Tentative de résolution du "
		      << "service de nommage. Hôte : " << host
		      << " Port : " << (unsigned long)port
		      << " ...";
		log (InformationLog (info1));
		NamingFacilities::initialize (_orb, host, port);
		info2 << "ORBFacilities::resolveCosNaming. Résolution du "
		      << "service de nommage. Hôte : " << host
		      << " Port : " << (unsigned long)port
		      << " effectuée avec succès.";
		log (InformationLog (info2));
	}
	catch (const IN_UTIL Exception& exc)
	{
		UTF8String	error (charset);
		error << "ORBFacilities::resolveCosNaming. Echec de la résolution du "
		      << "service de nommage. Hôte : " << host
		      << " Port : " << (unsigned long)port
		      << " : " << exc.getFullMessage ( );
		log (ErrorLog (error));
		throw;
	}
	catch (...)
	{
		UTF8String	error (charset);
		error << "ORBFacilities::resolveCosNaming. Echec de la résolution du "
		      << "service de nommage. Hôte : " << host
		      << " Port : " << (unsigned long)port
		      << " : Erreur non renseignée.";
		log (ErrorLog (error));
		throw;
	}
}	// ORBFacilities::resolveCosNaming


void ORBFacilities::resolveCosNaming ( )
{
	resolveCosNaming (getNSHostName ( ), getNSPort ( ));
}	// ORBFacilities::resolveCosNaming


void ORBFacilities::bindObjectToName (
						const string& objId, const string& objKind,
						CORBA::Object_ptr servant, bool rebind)
{
	if (false == NamingFacilities::isInited ( ))
		NamingFacilities::initialize (_orb);
	if (0 == servant)
		throw IN_UTIL Exception (UTF8String (
						"ORBFacilities::initialize : servant nul.", charset));
	NamingFacilities::bindObjectToName (objId, objKind, servant, rebind);
}	// ORBFacilities::bindObjectToName


Object_ptr ORBFacilities::getObjectReference (
						const string& objId, const string& objKind)
{
	if (false == NamingFacilities::isInited ( ))
		NamingFacilities::initialize (_orb);

	return NamingFacilities::getObjectReference(objId, objKind);
}	// ORBFacilities::getObjectReference


LogDispatcher* ORBFacilities::getLogDispatcher ( )
{
	return _logDispatcher;
}	// ORBFacilities::getLogDispatcher


void ORBFacilities::parseArgs (int argc, char* argv [])
{
	bool			hasNSHost	= false;
	const string	nsRefBase ("NameService=corbaname::");

	for (int i = 0; i < argc; i++)
	{
		if (i < argc - 1)
		{
			if (string ("-NS_HOST") == argv [i])
			{
				const string	host	= getGivenHostName (argv [++i]);
				if (host != _nsHostName)
				{
					_isDefaultNSHost	= false;
					_nsHostName			= host;
				}
				hasNSHost	= true;
			}
			else if (string ("-ORBInitRef") == argv [i])
			{
				i++;	// -NS_HOST prioritaire sur ORBInitRef
				if (0 == nsRefBase.compare (0, nsRefBase.length ( ), argv [i],
							0, nsRefBase.length ( )))
				{
					if (true == hasNSHost)
						continue;

					const string	host	= 
							getGivenHostName (string (argv [i]).substr (
														nsRefBase.length ( )));
					if (host != _nsHostName)
					{
						_nsHostName			= host;
						_isDefaultNSHost	= false;
					}
				}	// if (0 == nsRefBase.compare (0, nsRefBase.length ( ), ...
			}	// else if (string ("-ORBInitRef") == argv [i])
			else if (string ("-ORBgiopMaxMsgSize") == argv [i])
			{
				try
				{
					_giopMaxMsgSize	= 
						IN_UTIL NumericConversions::strToULong (argv [++i]);
				}
				catch (const IN_UTIL Exception& exc)
				{
					
					UTF8String	message (charset);
					message<< "Exception levée lors de l'analyse de l'argument "
					     << "-ORBgiopMaxMsgSize " << argv [i] << " : " 
					     << exc.getFullMessage ( );
					ConsoleOutput::cout ( ) << message << co_endl;
				}
				catch (...)
				{
					UTF8String	message (charset);
					message << "Exception non documentée lors de l'analyse de "
					     << "l'argument -ORBgiopMaxMsgSize " << argv [i]; 
					ConsoleOutput::cout ( ) << message << co_endl;
				}
			}	// else if (string ("-ORBgiopMaxMsgSize") == argv [i])
			else if (string ("-ORBofferBiDirectionalGIOP") == argv [i])
			{
				try
				{
					long	value	=
							IN_UTIL NumericConversions::strToLong (argv [++i]);
					_enableBidirPOA	= 0 == value ? false : true;
				}
				catch (const IN_UTIL Exception& exc)
				{
					UTF8String	message (charset);
					message <<"Exception levée lors de l'analyse de l'argument "
					     << "-ORBofferBiDirectionalGIOP " << argv [i] << " : " 
					     << exc.getFullMessage ( );
					ConsoleOutput::cout ( ) << message << co_endl;
				}
				catch (...)
				{
					UTF8String	message (charset);
					message << "Exception non documentée lors de l'analyse de "
					     << "l'argument -ORBofferBiDirectionalGIOP "
					     << argv [i]; 
					ConsoleOutput::cout ( ) << message << co_endl;
				}
			}	// else if (string ("-ORBofferBiDirectionalGIOP") == argv [i])
			else if (string ("-csPolicy") == argv [i])
			{
				i++;
				if (string ("cosNaming") == argv [i])
					_csPolicy	= ORBFacilities::CORBA_CS_COS_NAMING;
				else if (string ("direct") == argv [i])
					_csPolicy	= ORBFacilities::CORBA_CS_DIRECT;
				else if (string ("directBSD") == argv [i])
					_csPolicy	= ORBFacilities::CORBA_CS_BSD_DIRECT;
				else if (string ("reverse") == argv [i])
					_csPolicy	= ORBFacilities::CORBA_CS_REVERSE;
				else
				{
					UTF8String	message (charset);
					message << "Argument invalide (" << argv [--i]
					     << ") pour l'option -csPolicy.";
					ConsoleOutput::cout ( ) << message << co_endl;
				}
			}	// else if (string ("-csPolicy") == argv [i])
			else if (string ("-NS_PORT") == argv [i])
			{
				try
				{
					const size_t	port	= 
							IN_UTIL NumericConversions::strToULong (argv [++i]);
					if (port != _nsPort)
					{
						_isDefaultNSPort	= false;
						_nsPort				= port;
					}
				}
				catch (const IN_UTIL Exception& exc)
				{
					UTF8String	message (charset);
					message <<"Exception levée lors de l'analyse de l'argument "
					     << "-NS_PORT " << argv [i] << " : " 
					     << exc.getFullMessage ( );
					ConsoleOutput::cout ( ) << message << co_endl;
				}
				catch (...)
				{
					UTF8String	message (charset);
					message << "Exception non documentée lors de l'analyse de "
					     << "l'argument -NS_PORT " << argv [i]; 
					ConsoleOutput::cout ( ) << message << co_endl;
				}
			}	// else if (string ("-NS_PORT") == argv [i])
			else if (string ("-remoteLogStream") == argv [i])
				addRemoteLogStream (argv [i + 1]);
		}	// if (i < argc - 1)
	}	// for (int i = 0; i < argc; i++)
}	// ORBFacilities::parseArgs


void ORBFacilities::log (const Log& log)
{
	if (0 == _logDispatcher)
		throw IN_UTIL Exception (UTF8String (
			"ORBFacilities::log : absence de gestionnaire de logs.", charset));

	_logDispatcher->log (log);
}	// ORBFacilities::log


void ORBFacilities::addRemoteLogStream (const string& name)
{
	if (0 != getLogDispatcher ( ))
	{
		UTF8String	trace1Msg (charset), trace2Msg (charset);
		trace1Msg << "Ajout du flux de messages " << name << " ...";
		TRACE_LOG_5 (trace1, trace1Msg)
		log (trace1);
		getLogDispatcher ( )->addStream (
				DefaultLogFactory::createOutputStream (name, Log::ALL_KINDS)
					);
		trace2Msg << "Flux de messages " << name << " ajouté.";
		TRACE_LOG_5 (trace2, trace2Msg)
		log (trace2);
	}
	else
	{
		UTF8String	message (charset);
		message
			<< "ORBFacilities : impossibillité d'ajouter le flux de messages "
			<< name << " : absence de gestionnaire de logs.";
		throw IN_UTIL Exception (message);
	}
}	// ORBFacilities::addRemoteLogStream


}	// namespace CorbaFacilities

