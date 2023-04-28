#ifndef UTIL_INFOS_H
#define UTIL_INFOS_H

#include <TkUtil/util_config.h>
#include <TkUtil/Version.h>

/*!
 * \mainpage	Page principale de la bibliothèque TkUtil.
 *
 * \section	presentation	Présentation
 * La bibliothèque <I>tkutil</I> propose des classes utilitaires telles que :
 * <UL>
 * <LI>Classe d'<B>exception</B> Exception : ce type d'exception est
 * <B>susceptible d'être levée par la plupart des méthodes</B> de l'ensemble des
 * bibliothèques du paquetage <I>Toolkit</I>,
 * <LI>Deux classes <B>chaîne de caractères</B> supportant l'encodage
 * <I>unicode</I> : <I>UTF8String</I> (UTF-8) et <I>UnicodeString</I>(UTF-16,
 * classe obsolète),
 * <LI>Des classes liées au <B>système</B> : File, OperatingSystem,
 * Timer, UserData, Date, Locale,
 * <LI>Des classes liées aux <B>processus</B> : Process, ProcessMemory,
 * FileRedirection, RemoteProcess, PrintCommand,
 * <LI>Des classes liées au <B>réseau</B> : NetworkData,
 * <LI>Des classes <B>informatiques</B> : AutoArrayPtr, LoadableFileOption,
 * NumericConversions, Version,
 * Des classes informatiques de <B>gestion de traces (<I>logs</I>)</B> : Log,
 * LogOutputStream, DefaultLogFactory,
 * <LI>Des classes de gestion d'instances par <B>compteur de références</B> 
 * avec observateurs: ReferencedObject,
 * <LI>Des classes pour la programmation <B>multithread</B> :
 * <I>ThreadPool</I> (gestionnaire de threads) et <I>TaskIfc</I> (taches
 * exécutées par le gestionnaire de threads), et des classes devenues obsolètes
 * au profit de l'API C++ (<I>ThreadIfc</I>, <I>ThreadManager</I>, <I>Mutex</I>,
 * <I>ParallelTasksDispatcher</I>).
 * </UL>
 *
 */


BEGIN_NAMESPACE_UTIL


/**
 * Classe permettant de connaître la version de cette bibliothèque.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class UtilInfos
{
	public :

	/**
	 * @return		La version de cette bibliothèque
	 */
	static const Version& getVersion ( )
	{ return _version; }


	protected :

	/**
	 * Constructeurs, opérateur = et destructeur : interdits.
	 */
	UtilInfos ( );
	UtilInfos (const UtilInfos& exc);
	UtilInfos& operator = (const UtilInfos& exc);
	~UtilInfos ( );


	private :

	/**
	 * La version de cette bibliothèque.
	 */
	static const Version		_version;
};	// class UtilInfos


END_NAMESPACE_UTIL


#endif	// UTIL_INFOS_H
