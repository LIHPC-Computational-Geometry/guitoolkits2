#ifndef PROCESS_LOG_H
#define PROCESS_LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLog.h>
#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Classe représentant les sorties d'un processus.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class ProcessLog : public DefaultLog
{
	public :

	/**
	 * @param		Nom du processus
	 * @param		Sorties du processus
	 */
	ProcessLog (const UTF8String& process, const UTF8String& outputs);

	/**
	 * @param		Nom du processus
	 * @param		Sorties du processus au format ASCII
	 */
	ProcessLog (const UTF8String& process, const IN_STD string& outputs);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	ProcessLog (const ProcessLog&);
	ProcessLog& operator = (const ProcessLog&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~ProcessLog ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const;

	/**
	 * @return		Le nom du processus.
	 */
	virtual const UTF8String& getProcessName ( ) const
	{ return _processName; }


	private :

	/**
	 * Le nom du processus.
	 */
	UTF8String			_processName;
};	// class ProcessLog


END_NAMESPACE_UTIL


#endif	// PROCESS_LOG_H
