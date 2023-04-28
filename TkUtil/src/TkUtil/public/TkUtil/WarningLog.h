#ifndef WARNING_LOG_H
#define WARNING_LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLog.h>
#include <TkUtil/Exception.h>

#include <string>
#include <stdexcept>


BEGIN_NAMESPACE_UTIL


/**
 * Classe représentant des logs contenant des messages d'avertissement.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class WarningLog : public DefaultLog
{
	public :

	/**
	 * Constructeurs.
	 * @param		Message représenté, porté par une chaine de caractères ou
	 *				par une exception.
	 */
	WarningLog (const UTF8String& text);
	WarningLog (const IN_STD string& asciiText);
	WarningLog (const IN_STD exception& exc);
	WarningLog (const Exception& exc);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	WarningLog (const WarningLog&);
	WarningLog& operator = (const WarningLog&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~WarningLog ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const;
};	// class WarningLog


END_NAMESPACE_UTIL


#endif	// WARNING_LOG_H
