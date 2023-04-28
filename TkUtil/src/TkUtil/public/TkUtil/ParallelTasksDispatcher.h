#ifndef PARALLEL_TASKS_DISPATCHER_H
#define PARALLEL_TASKS_DISPATCHER_H

#include <TkUtil/util_config.h>
#include <TkUtil/MachineData.h>

#include <algorithm>
#include <vector>
#include <assert.h>


BEGIN_NAMESPACE_UTIL


/**
 * <P>Classe permettant de répartir des tâches parallèles en fonction de leur
 * poids.
 * </P>
 *
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 * @warning	<B>La méthode <I>getTasksRepartition</I>, sur laquelle tout repose,
 *			est à optimiser fortement.
 */
template <class Task> class ParallelTasksDispatcher
{
	public :

	/**
	 * Constructeur.
	 * @param		Nombre de processeurs à prendre en considération. Si nul
	 *				le nombre de processeur pris en compte est celui de la 
	 *				machine.
	 */
	ParallelTasksDispatcher (size_t processorNum = 0);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~ParallelTasksDispatcher ( )
	{ }

	/**
	 * @return		Le nombre de processeurs utilisés.
	 */
	virtual size_t getProcessorNum ( ) const
	{ return _processorNum; }

	/**
	 * Ajoute une tâche à répartir.
	 * @param		Poids de la tâche
	 * @param		Tâche ajoutée
	 */
	virtual void addTask (size_t weight, Task* task);

	/**
	 * @return		La liste des tâches réparties sur le nombre de 
	 *				"processeurs".
	 */
	virtual IN_STD vector<IN_STD vector <Task*> > getTasksRepartition ( );

	/**
	 * @return		La liste des tâches à effectuer.
	 */
	const IN_STD vector< IN_STD pair<size_t, Task*> >& getTasks ( ) const
	{ return _tasks; }


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	ParallelTasksDispatcher (const ParallelTasksDispatcher&)
	{ assert (0 && "ParallelTasksDispatcher copy constructor is not allowed.");}
	ParallelTasksDispatcher& operator = (const ParallelTasksDispatcher&)
	{
		assert(0 && "ParallelTasksDispatcher copy constructor is not allowed.");
		return *this;
	}

	/** Le nombre de "processeurs" utilisés. */
	size_t							_processorNum;

	/** La liste des couples poids/tâche à répartir. */
	IN_STD vector< IN_STD pair<size_t, Task*> >	_tasks;
};	// class ParallelTasksDispatcher


template <class Task> bool operator < (
			const IN_STD pair<size_t, Task*>& l, IN_STD pair<size_t, Task*>& r)
{
	return l.first < r.first ? true : false;
}


template <class Task> ParallelTasksDispatcher<Task>::ParallelTasksDispatcher (
														size_t processorNum)
	: _processorNum (processorNum)
{
	if (0 == _processorNum)
		_processorNum	= IN_UTIL MachineData::instance ( ).getProcessorsNum( );

	if (0 == _processorNum)
		_processorNum	= 1;
}	// ParallelTasksDispatcher<Task>::ParallelTasksDispatcher


template <class Task> void ParallelTasksDispatcher<Task>::addTask (
													size_t weight, Task* task)
{
	_tasks.push_back (IN_STD pair<size_t, Task*> (weight, task));
}	// ParallelTasksDispatcher<Task>::addTask


template <class Task> IN_STD vector<IN_STD vector <Task*> >
						ParallelTasksDispatcher<Task>::getTasksRepartition ( )
{
	assert (0 < _processorNum);
	size_t	tasksNum	= _tasks.size ( ) > _processorNum ?
						  _processorNum : _tasks.size ( );
	IN_STD vector<IN_STD vector <Task*> >	repartition (tasksNum);

	IN_STD sort (_tasks.begin ( ), _tasks.end ( ));
	size_t	pos	= 0;
	for (typename IN_STD vector< IN_STD pair<size_t, Task*> >::iterator it =
			_tasks.begin ( ); _tasks.end ( ) != it; it++, pos++)
		repartition [pos % tasksNum].push_back ((*it).second);

	return repartition;
}	// ParallelTasksDispatcher<Task>::getTasksRepartition


END_NAMESPACE_UTIL


#endif	// PARALLEL_TASKS_DISPATCHER_H
