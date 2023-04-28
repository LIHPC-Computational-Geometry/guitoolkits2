#ifndef LOCALIZED_ERROR_H
#define LOCALIZED_ERROR_H

#include <TkUtil/util_config.h>
#include <TkUtil/Exception.h>

#include <string>
#include <stdio.h>


BEGIN_NAMESPACE_UTIL


/**
 * Exception renseignant sur l'endroit d'où elle est survenue (fichier/ligne).
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class LocalizedError : public Exception
{
	public :

	/**
	 * Constructeur
	 * @param		Raison d'être de l'exception
	 * @param		Fichier où est levée l'exception (ex : __FILE__)
	 * @param		Ligne où est levée l'exception (ex : __LINE__)
	 */
	LocalizedError (const UTF8String& what, const IN_STD string fileName,
	                int line)
		: Exception (what), _fileName (fileName), _line (line)
	{ }

	/**
	 * Constructeur
	 * @param		Raison d'être de l'exception
	 * @param		Origine de l'exception
	 * @param		Fichier où est levée l'exception (ex : __FILE__)
	 * @param		Ligne où est levée l'exception (ex : __LINE__)
	 */
	LocalizedError (const UTF8String& what, const Exception& exc,
	                const IN_STD string fileName, int line)
		: Exception (what, exc), _fileName (fileName), _line (line)
	{ }

	/**
	 * Constructeur de copie et opérateur = : RAS
	 */
	LocalizedError (const LocalizedError& exc)
		: Exception (exc), _fileName (exc._fileName), _line (exc._line)
	{ }
	LocalizedError& operator = (const LocalizedError& exc)
	{
		if (&exc != this)
		{
			Exception::operator = (exc);
			_fileName	= exc._fileName;
			_line		= exc._line;
		}

		return *this;
	}

	/**
	 * Destructeur : ARS
	 */
	virtual ~LocalizedError ( )
	{ }

	/**
	 * @return		La raison d'être de l'exception précédée de la localisation
	 *				de la levée d'exception.
	 */
	virtual IN_UTIL UTF8String getFullMessage ( ) const;

	/**
	 * @return		Le fichier où est levée l'exception
	 */
	const IN_STD string& getFileName ( ) const
	{ return _fileName; }

	/**
	 * @return		La ligne où est levée l'exception
	 */
	int getLine ( ) const
	{ return _line; }


	private :

	IN_STD string		_fileName;
	int					_line;
};	// class LocalizedError


#ifndef LOCALIZED_ERROR
#define	LOCALIZED_ERROR(varName,what)	LocalizedError	varName(what, __FILE__, __LINE__);
#endif	// LOCALIZED_ERROR


END_NAMESPACE_UTIL


#endif	// LOCALIZED_ERROR_H
