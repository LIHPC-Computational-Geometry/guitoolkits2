#ifndef INTERNAL_ERROR_H
#define INTERNAL_ERROR_H

#include <TkUtil/util_config.h>
#include <TkUtil/LocalizedError.h>

#include <string>
#include <stdio.h>


BEGIN_NAMESPACE_UTIL


/**
 * Exception renseignant sur une erreur interne au logiciel.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class InternalError : public LocalizedError
{
	public :

	/**
	 * @param		Raison d'être de l'exception
	 * @param		Détails sur l'erreur rencontrée
	 * @param		Fichier où est levée l'exception
	 * @param		Ligne où est levée l'exception
	 */
	InternalError (const UTF8String& what, const UTF8String& detail,
	               const IN_STD string fileName, int line)
		: LocalizedError (what, fileName, line), _detail (detail)
	{ }

	/**
	 * @param		Raison d'être de l'exception
	 * @param		Détails sur l'erreur rencontrée
	 * @param		Fichier où est levée l'exception
	 * @param		Ligne où est levée l'exception
	 */
	InternalError (const UTF8String& what, const Exception& detail,
	               const IN_STD string fileName, int line)
		: LocalizedError (what, fileName, line),
		  _detail (detail.getFullMessage ( ))
	{ }

	/**
	 * @param		Raison d'être de l'exception
	 * @param		Détails sur l'erreur rencontrée
	 * @param		Fichier où est levée l'exception
	 * @param		Ligne où est levée l'exception
	 */
	InternalError (const UTF8String& what, const std::exception& detail,
	               const IN_STD string fileName, int line)
		: LocalizedError (what, fileName, line),
		  _detail (detail.what ( ))
	{ }

	/**
	 * Constructeur de copie et opérateur = : RAS
	 */
	InternalError (const InternalError& exc)
		: LocalizedError (exc), _detail (exc._detail)
	{ }
	InternalError& operator = (const InternalError& exc)
	{
		if (&exc != this)
		{
			LocalizedError::operator = (exc);
			_detail		= exc._detail;
		}

		return *this;
	}

	/**
	 * Destructeur : ARS
	 */
	virtual ~InternalError ( )
	{ }

	/**
	 * @return		La raison d'être de l'exception, précédée de sa
	 *				localisation, et suivie des détails.
	 */
	virtual IN_UTIL UTF8String getFullMessage ( ) const;

	/**
	 * @return		Les détails sur l'erreur rencontrée
	 */
	const IN_UTIL UTF8String& getDetail ( ) const
	{ return _detail; }


	private :

	IN_UTIL UTF8String		_detail;
};	// class InternalError


/**
 * Macro simplifiant la création d'une exception de type InternalError. Ex :
 * <P><CODE>
 * INTERNAL_ERROR(exc, "Ouverture du fichier impossible", "Droits en écriture
 * interdits")<BR>
 * throw exc;<BR>
 * </CODE></P>
 */
#ifndef INTERNAL_ERROR
#define	INTERNAL_ERROR(varName,what,detail)	IN_UTIL InternalError	varName(IN_UTIL UTF8String (what,IN_UTIL UTF8String::defaultCharset), IN_UTIL UTF8String (detail,IN_UTIL UTF8String::defaultCharset),__FILE__, __LINE__);
#define	INTERNAL_EXCEPTION(varName,what,detail)	IN_UTIL InternalError	varName(IN_UTIL UTF8String (what,IN_UTIL UTF8String::defaultCharset), detail,__FILE__, __LINE__);
#endif	// INTERNAL_ERROR


END_NAMESPACE_UTIL


#endif	// INTERNAL_ERROR_H
