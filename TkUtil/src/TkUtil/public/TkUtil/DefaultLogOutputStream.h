#ifndef DEFAULT_LOG_OUTPUT_STREAM_H
#define DEFAULT_LOG_OUTPUT_STREAM_H

#include <TkUtil/util_config.h>
#include <TkUtil/LogOutputStream.h>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe abstraite représente un flux sortant de logs et en 
 * implémentant les mécanismes de base.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class DefaultLogOutputStream : public LogOutputStream
{
	public :

	/**
	 * Destructeur. RAS.
	 */
	virtual ~DefaultLogOutputStream ( );

	/**
	 * Le jeu de caractères dans lequel les écritures sont faites.
	 */
	virtual const IN_UTIL Charset& getCharset ( ) const			// v 4.0.0
	{ return _charset; }
	virtual void setCharset (const IN_UTIL Charset& charset)	// v 4.0.0
	{ _charset	= charset; }

	/**
	 * Faut il lever une exception en cas d'erreur ? Non par défaut.
	 */
	virtual bool throwExceptionIfFails ( ) const // v 4.0.0
	{ return _throwException; }
	virtual void setThrowExceptionIfFails (bool throwIfFails)	// v 4.0.0
	{ _throwException	= throwIfFails; }

	/** Le masque permettant de définir le type de logs à enregistrer. 
	 * Ce masque fonctionne avec des opérations & et | sur les types
	 * élémentaires de logs.
	 */
	virtual Log::TYPE getMask ( ) const
	{ return _mask; }
	virtual void setMask (Log::TYPE mask)
	{ _mask	= mask; }

	/**
	 * @param		true si la date doit être affichée, sinon false.
	 */
	virtual void enableDate (bool enable)
	{ _enableDate	= enable; }

	/**
	 * @param		true si l'heure doit être affichée, sinon false.
	 */
	virtual void enableTime (bool enable)
	{ _enableTime	= enable; }

	/**
	 * @param		true si la date et l'heure doivent être affichés,
	 *				sinon false.
	 */
	virtual void enableDate (bool date, bool time)
	{ _enableDate	= date;	_enableTime	= time; }

	/**
	 * @return		true si la date doit être affichée, sinon false.
	 */
	virtual bool displayDate ( ) const
	{ return _enableDate; }

	/**
	 * @return		true si l'heure doit être affichée, sinon false.
	 */
	virtual bool displayTime ( ) const
	{ return _enableTime; }

	/**
	 * @param		true si l'ID du thread doit être affiché, sinon false.
	 * @warning		requiert que la macro-définition
	 * 				<I>MULTITHREADED_APPLICATION</I> soit définit.
	 */
	virtual void enableThreadID (bool enable)
	{ _enableThreadID	= enable; }

	/**
	 * @return		true si l'ID du thread doit être affiché, sinon false.
	 * @warning		requiert que la macro-définition
	 * 				<I>MULTITHREADED_APPLICATION</I> soit définit.
	 */
	virtual bool displayThreadID ( ) const
	{ return _enableThreadID; }


	protected :

	/**
	 * Constructeur
	 * @param		Jeu de caractères à utiliser lors des écritures
	 * @param		True si une exception doit être levée en cas d'erreur
	 * 				d'écriture
	 * @param		Masque à appliquer
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de threads doivent être affichés
	 */
	DefaultLogOutputStream (
			const IN_UTIL Charset& charset,	// v 4.0.0
			bool throwExceptionIfFails,		// v 4.0.0
			Log::TYPE mask = Log::ALL_KINDS, bool enableDate = true,
			bool enableTime = true, bool enableThreadID = true);

	/**
	 * Constructeur
	 * @param		Masque à appliquer
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de threads doivent être affichés
	 */
	DefaultLogOutputStream (
			Log::TYPE mask = Log::ALL_KINDS, bool enableDate = true,
			bool enableTime = true, bool enableThreadID = true);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	DefaultLogOutputStream (const DefaultLogOutputStream&);
	DefaultLogOutputStream& operator = (const DefaultLogOutputStream&);

	/**
	 * Formate le texte du log reçu en argument pour affichage dans le flux.
	 * Ce formatage ajoute date et heure si nécessaire.
	 * @param		Log à afficher
	 * @return		texte à afficher, après formattage
	 * @see			displayDate
	 * @see			displayTime
	 */
	virtual UTF8String format (const Log& log) const;

	/**
	 * Formate le texte du log reçu en argument pour affichage dans le flux avec
	 * le jeu de caractères transmis en argument.
	 * Ce formatage ajoute date et heure si nécessaire.
	 * @param		Log à afficher
	 * @param		Jeu de caractères cible.
	 * @return		texte à afficher, après formattage
	 * @see			displayDate
	 * @see			displayTime
	 */
	virtual IN_STD string format (	// v 4.0.0
					const Log& log, const IN_UTIL Charset& charset) const;

	/** Le jeu de caractères utilisé pour les écritures. */
	IN_UTIL Charset			_charset;				// v 4.0.0

	/** Faut il lever une exception en cas d'erreur d'écriture ? */
	bool					_throwException;		// v 4.0.0

	/** Le masque appliqué. */
	Log::TYPE				_mask;

	/** Les dates doivent elles être affichées ? */
	bool					_enableDate;

	/** Les heures doivent elles être affichées ? */
	bool					_enableTime;

	/** Les IDs de threads doivent ils être affichés ? */
	bool					_enableThreadID;
};	// class LogOutputStream


END_NAMESPACE_UTIL


#endif	// DEFAULT_LOG_OUTPUT_STREAM_H
