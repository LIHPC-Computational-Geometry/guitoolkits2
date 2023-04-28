#ifndef LOG_DISPATCHER_H
#define LOG_DISPATCHER_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLogOutputStream.h>

#include <vector>


BEGIN_NAMESPACE_UTIL


/**
 * Gestionnaire d'instances de LogOutputStream. Dispatche auprès de chacun
 * des flux les logs reçus après filtrage par son masque.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class LogDispatcher : public DefaultLogOutputStream
{
	public :

	/**
	 * Constructeur.
	 * @param		Masque appliqué.
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de threads doivent être affichés
	 */
	LogDispatcher (
			Log::TYPE mask = Log::ALL_KINDS, bool enableDate = true,
			bool enableTime = true, bool enableThreadID = true);

	/**
	 * Destructeur. Détruit les flux qui lui sont rattachés.
	 * @see			addStream
	 */
	virtual ~LogDispatcher ( );

	/**
	 * Propage la requête à l'ensemble des flux.
	 * @param		true si la date doit être affichée, sinon false.
	 */
	virtual void enableDate (bool enable);

	/**
	 * Propage la requête à l'ensemble des flux.
	 * @param		true si l'heure doit être affichée, sinon false.
	 */
	virtual void enableTime (bool enable);

	/**
	 * Propage la requête à l'ensemble des flux.
	 * @param		true si la date et l'heure doivent être affichés,
	 *				sinon false.
	 */
	virtual void enableDate (bool enableDate, bool enableTime);

	/**
	 * Propage la requête à l'ensemble des flux.
	 * @param		true si l'ID du thread doit être affiché, sinon false.
	 * @warning		requiert que la macro-définition
	 * 				<I>MULTITHREADED_APPLICATION</I> soit définit.
	 */
	virtual void enableThreadID (bool enable);

	/**
	 * @return		false si au moins un flux n'est pas en état de 
	 *				fonctionnement.
	 */
	virtual bool isEnabled ( ) const;

	/**
	 * Transmet le log, si le masque l'autorise, à l'ensemble des flux
	 * gérés.
	 */
	virtual void log (const Log& log);

	/**
	 * Ajoute un LogOutputStream à la liste des flux gérés.
	 * @warning		Ce flux sera détruit par cette instance.
	 * @see			releaseStream
	 */
	virtual void addStream (LogOutputStream* stream);

	/**
	 * Libère le flux transmis en argument de la liste des flux gérés.
	 * @warning		Ne fait rien si le flux tranmis ne figure pas dans la
	 *				liste des flux gérés.
	 * @param		Flux à libérer.
	 * @see			addStream
	 */
	virtual void releaseStream (const LogOutputStream* stream);


	protected :

	/**
	 * Constructeur de copie et opérateur = : interdits. */
	LogDispatcher (const LogDispatcher&);
	LogDispatcher& operator = (const LogDispatcher&);


	private :

	/** Les flux gérés par cette instance. */
	IN_STD vector <LogOutputStream*>	_streams;
};	// class LogDispatcher


END_NAMESPACE_UTIL


#endif	// LOG_DISPATCHER_H
