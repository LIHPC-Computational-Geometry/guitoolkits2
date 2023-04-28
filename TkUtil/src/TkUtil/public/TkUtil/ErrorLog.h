#ifndef ERROR_LOG_H
#define ERROR_LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLog.h>
#include <TkUtil/Exception.h>
#include <string>
#include <stdexcept>


BEGIN_NAMESPACE_UTIL


/**
 * Classe représentant des logs contenant des messages d'erreur.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class ErrorLog : public DefaultLog
{
	public :

	/**
	 * Constructeurs.
	 * @param		Le message d'erreur, porté par une chaine de caractère ou
	 *				par une exception.
	 */
	ErrorLog (const UTF8String& text);
	ErrorLog (const IN_STD string& asciiText);
	ErrorLog (const IN_STD exception& exc);
	ErrorLog (const Exception& exc);

	/**
	 * Constructeur de copie et opérateur = : RAS. 
	 */
	ErrorLog (const ErrorLog&);
	ErrorLog& operator = (const ErrorLog&);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~ErrorLog ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const;
};	// class ErrorLog

END_NAMESPACE_UTIL



#endif	// ERROR_LOG_H
