#ifndef PROGRESSION_LOG_H
#define PROGRESSION_LOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLog.h>

#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Classe représentant des logs de progression d'une tache.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class TaskProgressionLog : public DefaultLog
{
	public :

	/**
	 * @param		Tache en cours
	 * @param		pourcentage d'avancement
	 * @param		nombre de décimales à afficher dans le message
	 */
	TaskProgressionLog (
			const UTF8String& task, double percent, unsigned short dec = 2);

	/**
	 * @param		Message comprenant tache + avancement.
	 */
	TaskProgressionLog (const UTF8String& message);

	/**
	 * @param		Message comprenant tache + avancement au format ASCII.
	 */
	TaskProgressionLog (const IN_STD string& message);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	TaskProgressionLog (const TaskProgressionLog&);
	TaskProgressionLog& operator = (const TaskProgressionLog&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~TaskProgressionLog ( );

	/**
	 * @return		Une copie de l'instance. Cette copie est à prendre en charge
	 *				par l'appelant, y compris sa destruction.
	 */
	virtual Log* clone ( ) const;

	/**
	 * @return		La tache effectuée
	 */
	virtual const UTF8String& getTask ( ) const
	{ return _task; }

	/**
	 * @return		Le pourcentage d'avancement
	 */
	virtual double getProgression ( ) const
	{ return _percent; }


	private :

	/**
	 * La tache
	 */
	UTF8String				_task;

	/**
	 * Le pourcentage d'avancement
	 */
	double					_percent;
};	// class TaskProgressionLog


END_NAMESPACE_UTIL


#endif	// PROGRESSION_LOG_H
