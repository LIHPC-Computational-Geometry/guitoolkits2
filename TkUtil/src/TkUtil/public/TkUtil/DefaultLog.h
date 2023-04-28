#ifndef DEFAULT_LOG_H
#define DEFAULT_LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/Log.h>

BEGIN_NAMESPACE_UTIL


/**
 * Classe abstraite représentant des logs et proposant les services de base.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 * @see		Log
 */
class DefaultLog : public Log
{
	public :

	/**
	 * Destructeur. RAS.
	 */
	virtual ~DefaultLog ( );

	/**
	 * @return		Le texte du log.
	 */
	virtual const UTF8String& getText ( ) const
	{ return _text; }

	/**
	 * @param		Nouveau texte du log.
	 */
	virtual void setText (const UTF8String& text);

	/**
	 * @return		La nature du log.
	 */
	virtual TYPE getType ( ) const
	{ return _type; }

	/**
	 * @return		La date de création du log.
	 */
	virtual const Date& getDate ( ) const
	{ return _date; }

#ifdef MULTITHREADED_APPLICATION

	/**
	 * @return		Le thread de création du log.
	 */
	virtual pthread_t getThreadId ( ) const
	{ return _threadId; }

#endif	// MULTITHREADED_APPLICATION


	protected :

	/**
	 * Constructeur.
	 * @param		Nature du log.
	 * @param		Texte du log.
	 */
	DefaultLog (TYPE type, const UTF8String& text);

	/**
	 * Constructeur.
	 * @param		Nature du log.
	 * @param		Texte (format ASCII) du log.
	 */
	DefaultLog (TYPE type, const IN_STD string& text);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	DefaultLog (const DefaultLog&);
	DefaultLog& operator = (const DefaultLog&);


	private :

	/** Le type du log. */
	TYPE					_type;

	/** Le contenu du log. */
	UTF8String				_text;

	/** La date de création du log. */
	Date					_date;

#ifdef MULTITHREADED_APPLICATION

	/** Le thread de création du log. */
	pthread_t				_threadId;

#endif	// MULTITHREADED_APPLICATION
};	// class DefaultLog


END_NAMESPACE_UTIL

#endif	// DEFAULT_LOG_H
