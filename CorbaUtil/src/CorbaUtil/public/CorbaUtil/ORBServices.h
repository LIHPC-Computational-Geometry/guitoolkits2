/**
 * ORBServices.h : utilitaires CORBA.
 * @author	: Charles PIGNEROL, CEA/DAM/DSSI
 */

#ifndef ORB_SERVICES_H
#define ORB_SERVICES_H

#include <TkUtil/Exception.h>
#include "ApplicationException.hh"
#include <TkUtil/Version.h>
#include <TkUtil/MemoryError.h>

#include <CORBA.h>
#ifdef OMNIORB
#include	<acconfig.h>		// PACKAGE_NAME, PACKAGE_VERSION
#include 	<omniORB4/omniInterceptors.h>
#else
#	error "Undefined ORB"
#endif


/*!
 * \mainpage    Page principale de la bibliothèque CorbaUtil.
 *
 * \section presentation    Présentation
 * <P>La bibliothèque <I>CorbaUtil</I> propose des services de gestion simplifiée
 * d'un <B>ORB</B> et de son éventuel <B>service de nommage</B> associé.
 * La version actuelle ne supporte que l'ORB <I>omniORB</I>.
 * </P>
 * <P>Le fichier ORBServices.h</I> met à disposition des fonctions permettant :
 * <OL>
 * <LI>de connaître la machine locale,
 * <LI>de récupérer un objet d'implémentation à partir de sa référence CORBA,
 * <LI>des macros pour gérer les exceptions lors d'appels CORBA,
 * </OL>
 * <P>La classe <I>ORBFacilities</I> offre les services suivants :
 * <OL>
 * <LI>Gestion simplifiée d'un ORB (initialisation, finalisation, gestion du
 * système de nommage),
 * <LI>Référencement d'objets d'implémentations auprès de l'ORB, et
 * récupération de leur proxy,
 * <LI>Accès aux principales entités de l'ORB : POA racine, POA manager,
 * <LI>Possibilité d'afficher les messages de fonctionnement de l'ORB dans une
 * instance de la classe <I>LogDispatcher</I> (cf. bibliothèque <I>TkUtil</I>).
 * </OL>
 *
 * \section depend      Dépendances
 * Cette bibliothèque utilise la bibliothèque TkUtil.
 * Les directives de compilation et d'édition des liens de la bibliothèque
 * <I>TkUtil</I> s'appliquent également à celle-ci.
 */


namespace CorbaFacilities
{

/**
 * @param		Nom de machine
 * @return		le nom de machine reçu en argument ou le nom de la machine 
 *				locale si le nom reçu en argument est <I>localhost</I>.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
IN_STD string getGivenHostName (const IN_STD string& name);


/**
 * Fonction paramétrée permettant de convertir, au sein d'un serveur, une
 * référence obtenue par un appel CORBA vers l'objet d'implémentation
 * Associé.
 * @param	Portable Object Adapter utilisé
 * @param	Proxy dont on cherche l'instance d'implémentation
 * @param	En retour, l'instance d'implémentation, s'il y en a une
 * @throw	Une exception de type UTIL::Exception est levée en cas d'erreur
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
template<class R, class I> void referenceToImpl (PortableServer::POA_var& poa, R* reference, I*& i)
{
	if (true == CORBA::is_nil (poa))
		throw IN_UTIL Exception ("referenceToImpl. Nul POA.");
	if (true == CORBA::is_nil (reference))
		throw IN_UTIL Exception ("referenceToImpl. Nul reference.");
#ifdef OMNIORB
	PortableServer::ServantBase*	servant	= 
						poa->reference_to_servant (reference);
	CHECK_NULL_PTR_ERROR (servant)
	servant->_remove_ref ( );
	i	= dynamic_cast<I*>(servant);
#else
#	error "Undefined ORB"
#endif
}


/**
 * @return		Le nom de l'ORB utilisé.
 */
IN_STD string getORBName ( );


/**
 * @return		La version de l'ORB utilisé.
 */
IN_STD string getORBVersion ( );

/**
 * @return		La version de cette bibliothèque.
 */
const IN_UTIL Version& getVersion ( );


/**
 * @param	machine où fonctionne le service de nommage
 * @param	port de connection TCP/IP
 * @return	la chaine de caractères à transmettre en argument du lancement d'un
 *			ORB et permettant d'initialiser le service de nommage. Cette chaine
 *			est du type : -ORBInitRef NameService=corbaname::machine:port
 */
IN_STD string getNameServiceArgs (const IN_STD string& host, size_t port);

/**
 * @return		Une chaine de caractères contenant le message de l'exception
 *				donnée en argument.
 */
IN_STD string corbaExceptionToString (const CORBA::Exception& exc);

/**
 * Effectue une migration, dans la IOR, de l'objet d'une machine vers une autre.
 * @param	IOR modifiée
 * @param	Machine de remplacement.
 * @warning	Testé uniquement sur des IORs de profile socket unix
 * (TAG_OMNIORB_UNIX_TRANS, giop:unix:path).
 * Cela marche avec omniORB 4.0.7 pour une IOR, absence de vérification du nom
 * de la machine côté serveur semble-t-il. Rien ne garantie que cela
 * fonctionnera avec des versions ultérieures d'omniORB ou avec un autre ORB.
 */
void migrateIOR (IN_STD string& ior, const IN_STD string& newhost);

/**
 * Utilise pour profile, lors de l'encodage des IORs, des sockets de domaine
 * Unix.
 * @param	Nom de machine à utiliser lors de l'encodage des IORs.
 * @param	Nom de socket Unix à utiliser lors de l'encodage des IORs.
 * @warning	Repose sur <I>migrateIORInfos</I> et est donc spécifique à
 * 			omniORB.
 * @warning	S'applique à l'ensemble des IORs encodées.
 */
void useUnixDomainProfile (const std::string& hostname, const std::string& socketPath);
}	// namespace CorbaFacilities

#define BEGIN_CORBA_TRY_CATCH_BLOCK                                         \
	bool				hasError	= false;                                \
	IN_STD	string		errorMsg;                                           \
	try                                                                     \
	{
#ifdef OMNIORB
#define ORB_CATCH_BLOCK                                                     \
	}                                                                       \
	catch (omniORB::fatalException&	fe)                                     \
	{                                                                       \
		errorMsg	= "omniORB::fatalException : ";                         \
		errorMsg	+= IN_STD string (fe.errmsg ( )) + ".";                 \
		hasError	= true;
#endif	// OMNIORB


#define COMPLETE_CORBA_TRY_CATCH_BLOCK                                      \
	}                                                                       \
	catch (const CorbaFacilities::ApplicationException& appExc)             \
	{                                                                       \
		errorMsg	= appExc.fullMessage;                                   \
		hasError	= true;                                                 \
	}                                                                       \
	catch (const CORBA::Exception& corbaExc)                                \
	{                                                                       \
		errorMsg	= CorbaFacilities::corbaExceptionToString (corbaExc);   \
		hasError	= true;                                                 \
	}                                                                       \
	catch (const IN_STD exception& stdExc)                                  \
	{                                                                       \
		errorMsg	= stdExc.what ( );                                      \
		hasError	= true;                                                 \
	}                                                                       \
	catch (const IN_UTIL Exception& uExc)                                   \
	{                                                                       \
		errorMsg	= uExc.getFullMessage ( );                              \
		hasError	= true;                                                 \
	}                                                                       \
	catch (...)                                                             \
	{                                                                       \
		errorMsg	= "Erreur non documentée.";                             \
		hasError	= true;                                                 \
	}


/**
 * <P>Macros en complément à <I>BEGIN_CORBA_TRY_CATCH_BLOCK</I> et
 * <I>COMPLETE_CORBA_TRY_CATCH_BLOCK</I>.</P>
 * <P>Lève une exception de type <I>ApplicationException</I> si <B>hasError</B>
 * vaut <I>true</I>.<P>
 */
#ifndef THROW_APPLICATION_EXCEPTION_IF_ERROR

#define THROW_APPLICATION_EXCEPTION_IF_ERROR                                 \
    if (true == hasError)                                                    \
    {                                                                        \
        CorbaFacilities::ApplicationException	exc;                         \
        exc.what        = errorMsg.c_str ( );                                \
        exc.fullMessage = errorMsg.c_str ( );                                \
        throw exc;                                                           \
    }	/* if (true == hasError) */
#endif	// THROW_APPLICATION_EXCEPTION_IF_ERROR

#endif	// ORB_SERVICES_H
