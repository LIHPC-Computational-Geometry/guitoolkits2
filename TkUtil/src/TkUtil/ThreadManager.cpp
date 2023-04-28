#include "TkUtil/ThreadManager.h"
#include "TkUtil/MachineData.h"
#include "TkUtil/Exception.h"
#include "TkUtil/MemoryError.h"


#include <assert.h>
#include <string.h>
#include <errno.h>
#include <sched.h>


USING_STD
USING_UTIL



BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");

	
// ============================================================================
//                          LA CLASSE ThreadManager
// ============================================================================


ThreadManager*			ThreadManager::_instance	= 0;


ThreadManager::ThreadManager (size_t tasksNum)
	: _tasksNum (tasksNum),
	  _queuedTasks ( ), _runningTasks ( ), 
	  _mutex (new Mutex ( )), _tasksMutex (new Mutex ( ))
{
	CHECK_NULL_PTR_ERROR (_mutex)
	CHECK_NULL_PTR_ERROR (_tasksMutex)
}	// ThreadManager::ThreadManager


ThreadManager::ThreadManager (const ThreadManager&)
	: _queuedTasks ( ), _runningTasks ( ), 
	  _mutex (0), _tasksMutex (0)
{
	assert (0 && "ThreadManager copy constructor is not allowed.");
}	// ThreadManager::ThreadManager


ThreadManager& ThreadManager::operator = (const ThreadManager&)
{
	assert (0 && "ThreadManager::operator = is not allowed.");
	return *this;
}	// ThreadManager::operator =


ThreadManager::~ThreadManager ( )
{
	delete _mutex;
	delete _tasksMutex;
}	// ThreadManager::~ThreadManager ( )


void ThreadManager::initialize (size_t tasksNum)
{
	if (0 != _instance)
		throw Exception (UTF8String ("ThreadManager::initialize : API déjà initialisée.", charset));

	_instance	= new ThreadManager (
				0 == tasksNum ?
				MachineData::instance ( ).getProcessorsNum ( ) : tasksNum);
	CHECK_NULL_PTR_ERROR (_instance)
}	// ThreadManager::initialize


void ThreadManager::finalize ( )
{
	if (0 == _instance)
		throw Exception (UTF8String ("ThreadManager::initialize : API non initialisée.", charset));

	delete _instance;
	_instance	= 0;
}	// ThreadManager::finalize


ThreadManager& ThreadManager::instance ( )
{
	if (0 == _instance)
		throw Exception (UTF8String ("ThreadManager::instance : API non initialisée.", charset));

	return *_instance;
}	// ThreadManager::instance


void ThreadManager::addTask (ThreadIfc* thread, bool queued)
{
	AutoMutex	mutex (_tasksMutex);
	if (0 == thread)
		throw Exception (UTF8String ("ThreadManager::addTask : absence de tâche.", charset));

	_queuedTasks.push_back (thread);
	if (false == queued)	// v 5.13.0
		launchTasks ( );
}	// ThreadManager::addTask (ThreadIfc* thread)


void ThreadManager::taskCompleted (ThreadIfc* thread)
{
	AutoMutex	mutex (_tasksMutex);
	if (0 == thread)
		throw Exception (UTF8String ("ThreadManager::taskCompleted : absence de tâche.", charset));

	for (vector<ThreadIfc*>::iterator it = _runningTasks.begin ( );
	     _runningTasks.end ( ) != it; it++)
	{
		if (thread == *it)
		{
			if (true == thread->isJoinable ( ))
				delete thread;
			_runningTasks.erase (it);
			break;
		}	// if (thread == *it)
	}	// for (vector<ThreadIfc*>::iterator it = _runningTasks.begin ( );

	launchTasks ( );
}	// ThreadManager::taskCompleted


void ThreadManager::join ( )
{
	AutoMutex	mutex (_mutex);

	while (true == hasJoinableTasks ( ))
	{
		{
			AutoMutex	taskMutex (_tasksMutex);
			if ((0 == _runningTasks.size ( )) && (0 != _queuedTasks.size ( )))
			{
				launchTasks ( );
			}
		}

		errno	= 0;
		if (0 != _runningTasks.size ( ))
			if (0 != sched_yield ( ))
			{
				cout
				<< "ThreadManager::join. Erreur lors de l'appel de sched_yield : "
				<< strerror (errno) << endl;
			}	// if (0 != sched_yield ( ))
	}	// while (true == hasJoinableTasks ( ))
}	// ThreadManager::join


void ThreadManager::launchTasks ( )
{
	AutoMutex	mutex (_tasksMutex);
	while (_runningTasks.size ( ) < _tasksNum)
	{
		bool	ran	= false;
		for (vector<ThreadIfc*>::iterator it = _queuedTasks.begin ( );
		     (false == ran) && (_queuedTasks.end ( ) != it); )
//		     (false == ran) && (_queuedTasks.end ( ) != it); it++)
		{
			if (true == validateConcurrency ((*it)->getConcurrencyFlag ( )))
			{
				(*it)->startTask ( );
				_runningTasks.push_back (*it);
				// coverity : on fait it++ sur un iterateur détruit
				// => on utilise la valeur de retour de erase qui est l'élément
				// suivant (ou end).
				it	= _queuedTasks.erase (it);
				ran	= true;
			}	// if (true == validateConcurrency (...
			else
				it++;
		}	// for (vector<ThreadIfc*>::iterator it = _queuedTasks.begin ( );

		if (false == ran)
			break;
	}	// while (_runningTasks.size ( ) < _tasksNum)
}	// ThreadManager::launchTasks


bool ThreadManager::hasJoinableTasks ( ) const
{
	AutoMutex	mutex (_tasksMutex);
	for (vector<ThreadIfc*>::const_iterator itr = _runningTasks.begin ( );
	     _runningTasks.end ( ) != itr; itr++)
		if (true == (*itr)->isJoinable ( ))
			return true;
	for (vector<ThreadIfc*>::const_iterator itq = _queuedTasks.begin ( );
	     _queuedTasks.end ( ) != itq; itq++)
		if (true == (*itq)->isJoinable ( ))
			return true;

	return false;
}	// ThreadManager::hasJoinableTasks ( )


bool ThreadManager::validateConcurrency (size_t flag) const
{
	AutoMutex	mutex (_tasksMutex);

	if (0 == flag)
		return true;

	for (vector<ThreadIfc*>::const_iterator it = _runningTasks.begin ( );
	     _runningTasks.end ( ) != it; it++)
		if (0 != ((*it)->getConcurrencyFlag ( ) & flag))
			return false;

	return true;
}	// ThreadManager::validateConcurrency


END_NAMESPACE_UTIL
