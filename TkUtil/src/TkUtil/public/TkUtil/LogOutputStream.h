#ifndef LOG_OUTPUT_STREAM_H
#define LOG_OUTPUT_STREAM_H

#include <TkUtil/util_config.h>
#include <TkUtil/Log.h>
#include <TkUtil/UTF8String.h>

#ifdef MULTITHREADED_APPLICATION
#include <TkUtil/Mutex.h>

#define LOCK_LOG_STREAM IN_UTIL AutoMutex autoMutex (getMutex ( ));

#else	// MULTITHREADED_APPLICATION

#define LOCK_LOG_STREAM

#endif	// MULTITHREADED_APPLICATION


BEGIN_NAMESPACE_UTIL


/**
 * <P>Cette classe abstraite représente un flux sortant de logs.
 * Un tel flux, souvent persistant, peut être paramétré de manière
 * à n'enregistrer que certains types de logs.</P>
 *
 * <P>Cette classe fournit le nécessaire en environnement <B>multithread</B>
 * pour verrouiller des opérations.</P>
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class LogOutputStream
{
	public :

	/**
	 * <P>Destructeur : RAS.</P>
	 *
	 * <P>Dans la version multithread détruit le mutex de protextion du flux.
	 * </P>
	 */
	virtual ~LogOutputStream ( );

	/**
	 * @return	true si le flux est en état de fonctionnement, ou false
	 *			dans le cas contraire.
	 */
	virtual bool isEnabled ( ) const = 0;

	/**
	 * Le jeu de caractères dans lequel les écritures sont faites.
	 */
	virtual const IN_UTIL Charset& getCharset ( ) const = 0;		// v 4.0.0
	virtual void setCharset (const IN_UTIL Charset& charset) = 0;	// v 4.0.0

	/**
	 * Faut il lever une exception en cas d'erreur ? Non par défaut.
	 */
	virtual bool throwExceptionIfFails ( ) const = 0;	// v 4.0.0
	virtual void setThrowExceptionIfFails (bool) = 0;	// v 4.0.0

	/**
	 * Enregistre le log reçu en argument si le masque l'autorise. Susceptible
	 * de lever une exception en cas d'erreur.
	 * @see		throwExceptionIfFails
	 */
	virtual void log (const Log& log) = 0;

	/**
	 * Le masque permettant de définir le type de logs à enregistrer. 
	 * Ce masque fonctionne avec des opérations & et | sur les types
	 * élémentaires de logs.
	 */
	virtual Log::TYPE getMask ( ) const = 0;
	virtual void setMask (Log::TYPE mask) = 0;

	/**
	 * @param		true si la date doit être affichée, sinon false.
	 */
	virtual void enableDate (bool enable) = 0;

	/**
	 * @param		true si l'heure doit être affichée, sinon false.
	 */
	virtual void enableTime (bool enable) = 0;

	/**
	 * @param		true si la date et l'heure doivent être affichés,
	 *				sinon false.
	 */
	virtual void enableDate (bool date, bool time) = 0;

	/**
	 * @return		true si la date doit être affichée, sinon false.
	 */
	virtual bool displayDate ( ) const = 0;

	/**
	 * @return		true si l'heure doit être affichée, sinon false.
	 */
	virtual bool displayTime ( ) const = 0;

	/**
	 * @param		true si l'ID du thread doit être affiché, sinon false.
	 * @warning		requiert que la macro-définition
	 * 				<I>MULTITHREADED_APPLICATION</I> soit définit.
	 */
	virtual void enableThreadID (bool enable) = 0;

	/**
	 * @return		true si l'ID du thread doit être affiché, sinon false.
	 * @warning		requiert que la macro-définition
	 * 				<I>MULTITHREADED_APPLICATION</I> soit définit.
	 */
	virtual bool displayThreadID ( ) const = 0;


	protected :

	/**
	 * Formate le texte du log reçu en argument pour affichage dans le flux.
	 * Ce formatage ajoute date et heure si nécessaire.
	 * @param		Log à afficher
	 * @return		texte à afficher, après formattage
	 * @see			displayDate
	 * @see			displayTime
	 */
	virtual UTF8String format (const Log& log) const = 0;

	/**
	 * Formate le texte du log reçu en argument pour affichage dans le flux avec
	 * le jeu de caractères transmis en argument.
	 * Ce formatage ajoute date et heure si nécessaire.
	 * @param		Log à afficher
	 * @param		Jeu de caractères cible.
	 * @return		texte à afficher, après formattage
	 * @see			getCharset
	 * @see			displayDate
	 * @see			displayTime
	 */
	virtual IN_STD string format (	// v 4.0.0
					const Log& log, const IN_UTIL Charset& charset) const = 0;

#ifdef MULTITHREADED_APPLICATION
	/**
	 * @return		Dans la version multithread, retourne une référence sur un
	 *				mutex destiné à protéger certains accesseurs de cette
	 *				classe.
	 */
	virtual IN_UTIL Mutex* getMutex ( ) const;
#endif	// MULTITHREADED_APPLICATION

	/**
	 * <P>Constructeur, constructeur de copie et opérateur = : RAS.</P>
	 *
	 * <P>Dans la version multithread instancie le mutex de protextion du flux
	 * (constructeur par défaut, constructeur de copie).
	 * </P>
	 */
	LogOutputStream ( );
	LogOutputStream (const LogOutputStream&);
	LogOutputStream& operator = (const LogOutputStream&);


	private :

#ifdef MULTITHREADED_APPLICATION
	/** Dans la version multithread, mutex destiné à protéger certains
	 * accesseurs de cette classe. */
	mutable Mutex*					_mutex;
#endif	// MULTITHREADED_APPLICATION
};	// class LogOutputStream



LogOutputStream& operator << (LogOutputStream& stream, const Log& log);


END_NAMESPACE_UTIL

#endif	// LOG_OUTPUT_STREAM_H
