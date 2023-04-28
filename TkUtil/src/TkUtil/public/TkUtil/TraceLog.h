#ifndef TRACE_LOG_H
#define TRACE_LOG_H

#include <TkUtil/util_config.h>
#include "TkUtil/DefaultLog.h"
#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Macros d'instanciations de logs de type Trace.
 * @param		name est le nom de l'instance
 * @param		text est le message
 * @param		type est le niveau de trace souhaité
 */
#define TRACE_LOG(name, text, type)                                          \
TraceLog	name (IN_UTIL UTF8String (text, type, IN_UTIL UTF8String (__FILE__, __LINE__);

#define TRACE_LOG_1(name, text)                                              \
TraceLog	name (IN_UTIL UTF8String (text, IN_UTIL UTF8String::defaultCharset), Log::TRACE_1, IN_UTIL UTF8String (__FILE__, IN_UTIL UTF8String::defaultCharset), __LINE__);

#define TRACE_LOG_2(name, text)                                              \
TraceLog	name (IN_UTIL UTF8String (text, IN_UTIL UTF8String::defaultCharset), Log::TRACE_2, IN_UTIL UTF8String (__FILE__, IN_UTIL UTF8String::defaultCharset), __LINE__);

#define TRACE_LOG_3(name, text)                                              \
TraceLog	name (IN_UTIL UTF8String (text, IN_UTIL UTF8String::defaultCharset), Log::TRACE_3, IN_UTIL UTF8String (__FILE__, IN_UTIL UTF8String::defaultCharset), __LINE__);

#define TRACE_LOG_4(name, text)                                              \
TraceLog	name (IN_UTIL UTF8String (text, IN_UTIL UTF8String::defaultCharset), Log::TRACE_4, IN_UTIL UTF8String (__FILE__, IN_UTIL UTF8String::defaultCharset), __LINE__);

#define TRACE_LOG_5(name, text)                                              \
TraceLog	name (IN_UTIL UTF8String (text, IN_UTIL UTF8String::defaultCharset), Log::TRACE_5, IN_UTIL UTF8String (__FILE__, IN_UTIL UTF8String::defaultCharset), __LINE__);


/**
 * Classe représentant des logs de type "trace" dont la vocation est d'aider
 * à la mise au point de programmes.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class TraceLog : public DefaultLog
{
	public :

	/**
	 * Constructeur.
	 * @param		Message associé.
	 * @param		Type du log (TRACE_1 à TRACE_5)
	 * @param		Fichier de création du log
	 * @param		Ligne de création du log
	 */
	TraceLog (const IN_UTIL UTF8String& text, TYPE type = TRACE_1,
	          const IN_UTIL UTF8String& file =
					IN_UTIL UTF8String ("", IN_UTIL UTF8String::defaultCharset),
	          unsigned long line = 0);

	/**
	 * Constructeur.
	 * @param		Message associé au format ASCII.
	 * @param		Type du log (TRACE_1 à TRACE_5)
	 * @param		Fichier de création du log
	 * @param		Ligne de création du log
	 */
	TraceLog (const IN_STD string& text, TYPE type = TRACE_1,
	          const IN_UTIL UTF8String& file =
					IN_UTIL UTF8String ("", IN_UTIL UTF8String::defaultCharset),
	          unsigned long line = 0);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	TraceLog (const TraceLog&);
	TraceLog& operator = (const TraceLog&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~TraceLog ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const;

	/**
	 * @return		Le texte du log.
	 */
	virtual const IN_UTIL UTF8String& getText ( ) const
	{ return _localizedText; }

	/**
	 * @return		Le nom du fichier où a été créé le log.
	 */
	virtual IN_UTIL UTF8String getFile ( ) const
	{ return _fileName; }

	/**
	 * @return		La ligne dans getFile ( ) où a été créé le log.
	 */
	virtual unsigned long getLine ( ) const
	{ return _line; }

	/**
	 * @return		Le nom du fichier et la ligne où a été créé le log.
	 */
	virtual IN_UTIL UTF8String where ( ) const;


	private :

	/** Le fichier de création du log. */
	IN_UTIL UTF8String			_fileName;

	/** La ligne de création du log. */
	unsigned long		_line;

	/** Le text contenant la localisation de la création du log. */
	IN_UTIL UTF8String			_localizedText;
};	// class TraceLog


END_NAMESPACE_UTIL


#endif	// TRACE_LOG_H
