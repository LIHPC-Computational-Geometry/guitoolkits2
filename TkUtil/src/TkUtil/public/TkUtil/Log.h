#ifndef LOG_H
#define LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/Date.h>
#include <TkUtil/UTF8String.h>

#ifdef MULTITHREADED_APPLICATION
#include <pthread.h>
#endif	// MULTITHREADED_APPLICATION


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe abstraite représente des logs, informations destinées à
 * l'utilisateur ou à l'administrateur d'un logiciel.
 * Un log est caractérisé par son type, par son contenu et par sa date de
 * création.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Log
{
	public :

	/**
	 * Définit la nature du Log.
	 */
	typedef unsigned long	TYPE;

	/**
	 * Les différents types de logs :
	 * <UL>
	 * <LI>aucun type,
	 * <LI>type erreur,
	 * <LI>type avertissement,
	 * <LI>type information,
	 * <LI>type sortie d'un processus exécuté ou détaché,
	 * <LI>type scripting,
	 * <LI>type progression d'une tache,
	 * <LI>type trace (pour deboguer), niveaux 1 à 5,
	 * <LI>tous types de logs en mode production (pas de traces),
	 * <LI>tous types de logs de type trace,
	 * <LI>tout type de log.
	 * </UL>
	 */
	static const TYPE		NONE, ERROR, WARNING, INFORMATION, PROCESS,
							SCRIPTING, TASK_PROGRESSION,
							TRACE_1, TRACE_2, TRACE_3, TRACE_4, TRACE_5,
							PRODUCTION, ALL_TRACES, ALL_KINDS;

	/**
	 * Destructeur : RAS.
	 */
	virtual ~Log ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const = 0;

	/**
	 * @return		Le texte du log.
	 */
	virtual const UTF8String& getText ( ) const = 0;

	/**
	 * @param		Nouveau texte du log.
	 */
	virtual void setText (const UTF8String& text) = 0;

	/**
	 * @return		Le type du log.
	 */
	virtual TYPE getType ( ) const = 0;

	/**
	 * @return		La date de création du log.
	 */
	virtual const Date& getDate ( ) const = 0;

#ifdef MULTITHREADED_APPLICATION

	/**
	 * @return		Le thread de création du log.
	 */
	virtual pthread_t getThreadId ( ) const = 0;

#endif	// MULTITHREADED_APPLICATION


	protected :

	/**
	 * Constructeurs et opérateur = : RAS.
	 */
	Log ( );
	Log (const Log&);
	Log& operator = (const Log&);
};	// class Log


END_NAMESPACE_UTIL


#endif	// LOG_H
