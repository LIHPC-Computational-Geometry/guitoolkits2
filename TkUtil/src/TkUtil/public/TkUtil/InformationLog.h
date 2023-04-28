#ifndef INFORMATION_LOG_H
#define INFORMATION_LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLog.h>
#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Classe représentant des logs informatifs.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class InformationLog : public DefaultLog
{
	public :

	/**
	 * Constructeur.
	 * @param		Message associé.
	 */
	InformationLog (const UTF8String& text);

	/**
	 * Constructeur.
	 * @param		Message associé au format ASCII.
	 */
	InformationLog (const IN_STD string& text);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	InformationLog (const InformationLog&);
	InformationLog& operator = (const InformationLog&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~InformationLog ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const;
};	// class InformationLog


END_NAMESPACE_UTIL


#endif	// INFORMATION_LOG_H
