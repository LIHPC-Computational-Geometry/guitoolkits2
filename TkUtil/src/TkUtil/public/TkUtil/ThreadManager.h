#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <TkUtil/util_config.h>
#include <TkUtil/Threads.h>
#include <TkUtil/Mutex.h>

#include <string>
#include <assert.h>
#include <vector>


BEGIN_NAMESPACE_UTIL


/**
 * <P>======================================================================</P>
 * <P><B>ATTENTION :</B> Classe en cours de développement et de mise au
 * point.</P>
 * <P>======================================================================</P>
 *
 * <P>Cette classe permet de gérer un ensemble de <B>threads</B>. Elle lance
 * simultanément autant de threads que possible et, dès la fin d'un thread,
 * en relance un si nécessaire.
 * </P>
 *
 * <P>Cette classe permet de gérer des problèmes de compatibilité entre tâches.
 * </P>
 *
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 * @see		ThreadManager
 */
class ThreadManager
{
	public :

	/**
	 * Initialise cette classe.
	 * @param		Nombre de tâches à exécuter au maximum en concurrence. Si 0,
	 *				prend le nombre de processeurs de la machine.
	 * @warning		Fonction à appeler avant toute utilisation de cette classe.
	 */
	static void initialize (size_t tasksNum = 0);

	/**
	 * Détruit le manager.
	 * @warning		Ne plus utiliser ette classe après invocation de cette
	 *				fonction.
	 */
	static void finalize ( );
	
	/**
	 * @return		une référence sur l'unique instance de cette classe.
	 */
	static ThreadManager& instance ( );

	/**
	 * <P>Ajoute la tâche tranmise en argument à la liste des tâches à exécuter.
	 * Cette tâche sera lancée dès que possible. Si cette tâche est <B>joignable
	 * </B> elle sera alors <B>détruite automatiquement</B> par cette instance
	 * lorsqu'elle invoquera <I>taskCompleted</I> en fin d'exécution.
	 * @param		Tâche ajoutée
	 * @param		Si true la tâche n'est ajoutée qu'en file d'attente, et aucune tâche n'est éventuellement relancée.
	 * 				Le lancement des tâches peut alors se faire directement via ThreadManager::instance ( ).join ( ) une
	 * fois que toutes les tâches sont prêtes à être lancées.
	 * @see			taskCompleted
	 * @see			JoinableThread
	 */
	void addTask (ThreadIfc* thread, bool queued = false);

	/**
	 * <P>Méthode automatiquement appellée lorsqu'une tâche est finie. Met à 
	 * jour les listes de tâches et relance une tâche si nécessaire.</P>
	 * <P><B>Détruit les tâches joignables gérées par cette instance.</B></P>
	 * @param		Tâche terminée
	 * @see			addTask
	 * @see			JoinableThread
	 */
	void taskCompleted (ThreadIfc* thread);

	/**
	 * Attend la terminaison de tous les threads joignables avant de rendre la main.
	 * A noter que cette fonction lance les threads en file d'attente (cf. addTask).
	 * @warning		Les threads sont détruits à la fin de cette méthode.
	 * @see			JoinableThread
	 */
	void join ( );


	protected :

	/**
	 * Lance autant de tâches que possible.
	 */
	void launchTasks ( );

	/**
	 * @return		true s'il y a des threads joignables en cours d'exécution
	 *				ou en file d'attente, false dans le cas contraire.
	 */
	bool hasJoinableTasks ( ) const;

	/**
	 * @return		true si une tâche dont le drapeau de concurrence transmis
	 *				en argument peut être lancée, false dans le cas contraire.
	 */
	bool validateConcurrency (size_t flag) const;


	private :

	/**
	 * Constructeur. RAS.
	 * @param		Nombre de tâches à exécuter au maximum en concurrence.
	 */
	ThreadManager (size_t tasksNum);

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	ThreadManager (const ThreadManager&);
	ThreadManager& operator = (const ThreadManager&);

	/**
	 * Destructeur. RAS.
	 */
	~ThreadManager ( );


	/** L'éventuelle unique instance de cette classe. */
	static ThreadManager*				_instance;

	 /** Nombre de tâches à exécuter au maximum en concurrence. */
	size_t								_tasksNum;

	/** La liste des tâches à lancer. */
	IN_STD vector<ThreadIfc*>			_queuedTasks;

	/** La liste des tâches en cours. */
	IN_STD vector<ThreadIfc*>			_runningTasks;

	/** Le mutex de protection de l'instance vis à vis de l'extérieur. */
	mutable Mutex*						_mutex;

	/** Le mutex de protection de l'instance vis à vis des listes de tâches
	 * gérées. */
	mutable Mutex*						_tasksMutex;
};	// class ThreadManager


END_NAMESPACE_UTIL


#endif	// THREAD_MANAGER_H
