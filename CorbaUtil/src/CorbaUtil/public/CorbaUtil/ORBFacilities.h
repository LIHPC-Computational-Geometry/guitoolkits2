/**
 * ORBFacilities.h : services pour le service de nommage CORBA.
 * @author	: Charles PIGNEROL, CEA/DAM/DSSI
 */

#ifndef ORB_FACILITIES_H
#define ORB_FACILITIES_H

#include <string>
#include <CORBA.h>
#include <TkUtil/Exception.h>
#include <TkUtil/LogDispatcher.h>


namespace CorbaFacilities
{

/**
 * Classe encapsulant des fonctionnalités d'un ORB.
 * Ce service est utilisable dans le cadre d'applications simples
 * n'utilisant qu'un seul service de nommage sans contexte d'application.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class ORBFacilities
{
	public :

	/**
	 * CORBA_CS_POLICY : mode de fonctionnement client/serveur de l'application.
	 * - CORBA_CS_COS_NAMING : via API Cos Naming Corba
	 * - CORBA_CS_DIRECT : le client se connecte directement au serveur via string_to_object
	 * - CORBA_CS_BSD_DIRECT : le client se connecte directement au serveur via string_to_object. Connexion client/serveur via socket Unix BSD.
	 * - CORBA_CS_REVERSE : le serveur se connecte au client.
	 * @see		getCsPolicy
	 * @see		setCsPolicy
	 * @see		csPolicyToString
	 * @see		useSSHTunneling
	 */
	enum CORBA_CS_POLICY { CORBA_CS_COS_NAMING, CORBA_CS_DIRECT, CORBA_CS_BSD_DIRECT, CORBA_CS_REVERSE };
	
	/**
	 * <P>
	 * Initialise ce service. Ceci est un préalable avant toute utilisation
	 * d'une autre fonction de cette classe.
	 * </P>
	 * <P>
	 * Les arguments d'initialisation de l'ORB sont ceux définis par CORBA
	 * (options -ORBxxx valeur). Dans le cas du service de nommage les deux
	 * options ci-dessus surchargent celles par défaut de CORBA :
	 * <UL>
	 * <LI><I>-NS_HOST</I> : machine où est localisé le service de nommage (par
	 * défaut la machine locale),
	 * <LI><I>-NS_PORT</I> : port de connection au service de nommage (par 
	 * défaut 2809).
	 * </UL>
	 * </P>
	 * <P>Initialise le dispatcher de logs, accessible via la méthode
	 * getLogDispatcher. L'option de ligne de commande
	 * <I>-remoteLogStream option</I> met en place une sortie de log.
	 * <I>option</I> peut prendre les valeurs suivantes :
	 * <UL>
	 * <LI><I>stdout</I> : les logs de ce module seront envoyés sur la sortie
	 * standard du processus (cout),
	 * <LI><I>stderr</I> : les logs de ce module seront envoyés sur la sortie
	 * erreur du processus (cerr),
	 * <LI><I>fichier</I> : les logs de ce module seront envoyés dans le fichier
	 * qui sera préalablement vidé.
	 * </UL>
	 * </P>
	 * @param		nombre d'arguments (ex : argc du main).
	 * @param		arguments d'initialisation de l'ORB (ex : argv du main).
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 * @see			finalize
	 * @see			getLogDispatcher
	 */
	static void initialize (int argc, char* argv []);

	/**
	 * Idem initialize (int argc, char* argv []) mais en précisant ici le mode de fonctionnement
	 * à utiliser lors de l'initialisation de l'ORB. L'idée est ici par exemple d'avoir une application
	 * en mode <I>CORBA_CS_BSD_DIRECT</I> où, côté client, on initialise l'ORB avec pour mode
	 * <I>CORBA_CS_DIRECT</I>, le mode jouant ici, à l'initialisation, sur l'option <I>ORBendPoint</I>,
	 * et donc avec un <I>ORBendPoint</I> de type socket Unix côté serveur.
	 * @warning	<B>ATTENTION</B>, ne fait donc pas appel à <I>setCsPolicy (policy)</I>.</B>
	 */
	static void initialize (int argc, char* argv [], CORBA_CS_POLICY policy);	// v 5.4.0

	/**
	 * Boucle évènementielle de l'ORB.
	 */
	static void run ( );

	/**
	 * Libère les ressources utilisées par ce service.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 * @see			initialize
	 */
	static void finalize ( );

	/**
	 * @return		un pointeur sur l'ORB, ou 0 s'il n'est pas initialisé.
	 */
	static CORBA::ORB_ptr getORB ( );

	/**
	 * @return		un pointeur sur le POA racine.
	 * @warning		requiert un appel préalable à initialize.
	 * @see			getUsedPOA
	 */
	static PortableServer::POA_var& getRootPOA ( )
	{ return _rootPOA; }

	/**
	 * @return		un pointeur sur le POA bidirectionel. Le créé si
	 *				nécessaire.
	 * @see			getUsedPOA
	 */
	static PortableServer::POA_var& getBidirPOA ( );

	/**
	 * @return		Le POA utilisé par défaut. Ce POA est par défaut 
	 *				<I>getRootPOA</I>, mais peut être <I>getBidirPOA</I> si
	 *				l'ORB a été initialisé avec le couple d'arguments de ligne
	 *				de commande <I>-ORBofferBiDirectionalGIOP 1</I>.
	 * @see			getRootPOA
	 * @see			getBidirPOA
	 */
	static PortableServer::POA_var& getUsedPOA ( );

	/**
	 * @return		un pointeur sur le manager de POAs.
	 * @warning		requiert un appel préalable à initialize.
	 */
	static PortableServer::POAManager_var& getPOAManager ( )
	{ return _poaManager; }

	/**
	 * @return		Le mode de fonctionnement client/serveur utilisé.
	 * @see			setCsPolicy
	 * @see		useSSHTunneling
	 */
	static CORBA_CS_POLICY getCsPolicy ( )
	{ return _csPolicy; }

	/**
	 * @param		Le mode de fonctionnement client/serveur à utiliser.
	 * @see			getCsPolicy
	 */
	static void setCsPolicy (CORBA_CS_POLICY);

	/**
	 * @return		<I>true</I> si les communications clients/serveurs se font
	 * 				via un tunnel SSH (requiert le mode CORBA_CS_POLICY cÔté
	 * 				client et le mode CORBA_CS_BSD_POLICY côté serveur).
	 * @see	getCsPolicy
	 * @see	setSSHTunneling
	 */
	static bool useSSHTunneling ( )
	{ return _useTunneling; }

	/**
	 * @return		<I>true</I> si les communications clients/serveurs doivent
	 * 				être effectuées via un tunnel SSH (requiert le mode
	 * 				CORBA_CS_POLICY cÔté client et le mode CORBA_CS_BSD_POLICY
	 * 				côté serveur).
	 * @see	getCsPolicy
	 * @see	useSSHTunneling
	 */
	static void setSSHTunneling (bool on);

	/**
	 * Fonctions utilitaires mode de fonctionnement <-> chaine de caractères.
	 */
	static std::string csPolicyToString (CORBA_CS_POLICY);
	static CORBA_CS_POLICY stringToCsPolicy (const std::string&);

	/**
	 * @return		true si le sevice de nommage est initialisé, sinon false.
	 */
	static bool isNameServiceInited ( );

	/**
	 * @return		la machine (qui sera) utilisée par le service de nommage.
	 */
	static const std::string& getNSHostName ( );

	/**
	 * @return		true si la machine (qui sera) utilisée par le service de
	 *				nommage est celle par défaut (localhost), ou false si elle
	 *				a été spécifié (arguments de ligne de commande, 
	 *				resolveCosNaming).
	 */
	static bool isDefaultNSHost ( );

	/**
	 * @return		true si le port (qui sera) utilisé par le service de
	 *				nommage est celui par défaut (2809), ou false si il
	 *				a été spécifié (arguments de ligne de commande, 
	 *				resolveCosNaming).
	 */
	static bool isDefaultNSPort ( );

	/**
	 * @return		le port TCP/IP (qui sera) utilisé par le service de nommage.
	 */
	static size_t getNSPort ( );

	/**
	 * @param		Créé le nom de fichier si celui-ci n'a pas encore été
	 * 				affecté.
	 * @return		Le fichier qui sera utilisé en tant que socket de domaine 
	 *				Unix BSD.
	 * @see			setUnixSocketFileName
	 */
	static IN_STD string getUnixSocketFileName (bool createIfNecessary = false);

	/**
	 * @param		Le fichier qui sera utilisé en tant que socket de domaine 
	 *				Unix BSD.
	 * @see			getUnixSocketFileName
	 */
	static void setUnixSocketFileName (const IN_STD string& fileName);

	/**
	 * @return		La taille maximale des messages échangés entre serveur et
	 *				client. Cette valeur est obtenue durant <B>initialize</B>.
	 */
	static size_t getGiopMaxMsgSize ( );

	/**
	 * Fonction d'initialisation du service de nommage.
	 * @param		machine où est localisé le service de nommage
	 * @param		port TCP/IP de connexion au service de nommage
	 * @warning		Ne prend pas en compte le mode de fonctionnement de
	 * 				l'application (getCsPolicy ( )), c'est à l'application de
	 * 				le gérer.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 * @see			getCsPolicy
	 */
	static void resolveCosNaming (std::string host, size_t port);

	/**
	 * Fonction d'initialisation du service de nommage. Les paramètres utilisés
	 * sont ceux transmis à initialize, ou, à défaut, ceux par défaut.
	 * @warning		Ne prend pas en compte le mode de fonctionnement de
	 * 				l'application (getCsPolicy ( )), c'est à l'application de
	 * 				le gérer.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 * @see			initialize
	 * @see			getCsPolicy
	 */
	static void resolveCosNaming ( );

	/**
	 * Fonction d'enregistrement d'une instance d'une application auprès du
	 * service de nommage.
	 * @param		Identifiant de l'instance (name.id)
	 * @param		Type de l'instance (name.kind)
	 * @param		Objet à enregistrer. Doit être valide.
	 * @param		Si true, l'instance est enregistrée à nouveau si son nom est
	 *				déjà enregistré (rebind). Dans le cas contraire, une
	 *				exception est levée si son nom est déjà enregistré.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 */
	static void bindObjectToName (
					const std::string& objId, const std::string& objKind,
					CORBA::Object_ptr servant, bool rebind
						);

	/**
	 * Fonction dédiée pour retourner un proxy sur une instance d'un serveur.
	 * @param		Identifiant de l'instance (name.id)
	 * @param		Type de l'instance (name.kind)
	 * @return		Une référence sur l'instance demandée
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 */
	static CORBA::Object_ptr getObjectReference (
					const std::string& objId, const std::string& objKind
						);

	/**
	 * @return		Le dispatcher de logs de ce service. Ce dispatcher est
	 *				accessible après initialize et avant finalize.
	 */
	static IN_UTIL LogDispatcher* getLogDispatcher ( );

	/**
	 * Logue le message transmis en argument.
	 */
	static void log (const IN_UTIL Log& log);


	protected :

	/** Analyse des arguments transmis.
	 * @param		nombre d'arguments (ex : argc du main).
	 * @param		arguments d'initialisation de l'ORB (ex : argv du main).
	 */
	static void parseArgs (int argc, char* argv []);

	/**
	 * Ajoute un flux sortant de logs au dispatcher de logs. Ce flux affiche
	 * tous types de logs.
	 * @param		name est le nom du flux sortant à ajouter.
	 */
	static void addRemoteLogStream (const IN_STD string& name);

	/** L'ORB utilisé pendant la session de travail. */
	static CORBA::ORB_var					_orb;

	/** L'adapteur "racine". */
	static PortableServer::POA_var			_rootPOA;

	/** true si l'ORB a été initialisé avec les arguments
	 * <I>-ORBofferBiDirectionalGIOP 1</I>. */
	static bool								_enableBidirPOA;

	/** L'éventuel adapteur "communications bidirectionnelles". */
	static PortableServer::POA_var			_bidirPOA;

	/** Le manager de POAs. */
	static PortableServer::POAManager_var	_poaManager;

	/** Le mode fonctionnement en cours de l'application. CORBA_CS_BSD_DIRECT
	  * par défaut.  */
	static CORBA_CS_POLICY					_csPolicy;

	/** <I>true</I> si les communications client/serveur se font via un tunnel
	 * SSH. */
	static bool								_useTunneling;

	/** La machine (qui sera) utilisée par le service de nommage. */
	static std::string						_nsHostName;

	/** La machine du service de nommage est elle celle par défaut ? */
	static bool								_isDefaultNSHost;

	/** Le port TCP/IP (qui sera) utilisé par le service de nommage. */
	static size_t							_nsPort;

	/** Le port du service de nommage est il celui par défaut ? */
	static bool								_isDefaultNSPort;

	 /** Le fichier qui sera utilisé en tant que socket de domaine Unix BSD. */
	static IN_STD string					_unixSocketFileName;

	/** La taille maximale des messages échangés entre serveur et client. */
	static size_t							_giopMaxMsgSize;

	/** Le dispatcher de logs utilisé par ce service. */
	static IN_UTIL LogDispatcher*			_logDispatcher;
};	// class ORBFacilities


}	// namespace CorbaFacilities

#endif	// ORB_FACILITIES_H
