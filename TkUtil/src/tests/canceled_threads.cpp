#include "TkUtil/Exception.h"
#include "TkUtil/Mutex.h"
#include "TkUtil/NumericConversions.h"
#include "TkUtil/Threads.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>


USING_STD
USING_UTIL


/**
 * Classe écrivain partagé par des threads utilisant un flux C ou C++. Les
 * opérations d'écritures sont protégées par un mutex et ne peuvent pas faire
 * l'objet d'une annulation.
 */
class Writer
{
	public :

	/**
	 * @param		true s'il faut utitlsé un flux C (printf), false s'il faut
	 *				utiliser le flux C++ cout.
	 */
	Writer (bool cStream)
		: _cStream (cStream), _mutex ( )
	{ }

	~Writer ( )
	{ }

	/**
	 * Ecrit message dans le flux, précédé de l'id du thread qui doit être
	 * tranmis en premier argument.
	 */
//	void write (pthread_t id, const char* message);
	void write (pthread_t id, const string& message);


	private :

	Writer (const Writer& w)
		: _cStream (w._cStream), _mutex ( )
	{ }
	Writer& operator = (const Writer&)
	{ return *this; }

	bool	_cStream;
	Mutex	_mutex;
};	// class Writer


//void Writer::write (pthread_t id, const char* message)
void Writer::write (pthread_t id, const string& message)
{
	AutoMutex	am (&_mutex);

	if (true == _cStream)
		printf ("%u %s\n", id, message.c_str ( ));
	else
		cout << id << " " << message << endl;
}	// Writer::write


/**
 * Effectue une boucle infinie, avec une opération d'écriture toutes les
 * 10.000 itérations. Ce thread est de type annulable de manière asynchrone.
 */
class InfiniteLoopThread : public JoinableThread
{
	public :

	InfiniteLoopThread (Writer* writer);

	virtual ~InfiniteLoopThread ( );


	protected :

	virtual void execute ( );


	private :

	InfiniteLoopThread (const InfiniteLoopThread&)
	{ }
	InfiniteLoopThread& operator = (const InfiniteLoopThread&)
	{ return *this; }

	Writer*	_writer;
};	// class InfiniteLoopThread


InfiniteLoopThread::InfiniteLoopThread (Writer* writer)
	: JoinableThread ( ), _writer (writer)
{
}	// InfiniteLoopThread::InfiniteLoopThread


InfiniteLoopThread::~InfiniteLoopThread ( )
{
	if (0 != _writer)
		_writer->write (getId ( ), "Deadth of thread");
}	// InfiniteLoopThread::~InfiniteLoopThread


void InfiniteLoopThread::execute ( )
{
	if (0 != _writer)
		_writer->write (getId ( ), "In infinite loop.");
	for (size_t i = 0; i < ULONG_MAX; i++)
	{
		if (0 == (i % 10000))
		{
			if (0 != _writer)
				_writer->write (getId ( ), "");
//					_writer->write (getId ( ), NumericConversions::toStr (i));
		}	// if (0 == (i % 10000))

		if (true == cancelRequested ( ))
			break;
	}	// for (size_t i = 0; i < ULONG_MAX; i++)
}	// InfiniteLoopThread::execute


static int parseArgs (int argc, char* argv[]);
static int syntax (const string& pgm);


static size_t	tasksCount	= 5;	// Nombre de tâches concurrentes
static bool		cStream		= true;	// Ecritures dans printf (true) ou cout


int main (int argc, char* argv[])
{
	vector <InfiniteLoopThread*>	tasks;
	size_t							i		= 0;

	if (0 != parseArgs (argc, argv))
	 return syntax (argv [0]);
	cout << "Nombre de tâches concurrentes : " << tasksCount << endl
	     << "Flux d'écriture               : "
	     << (true == cStream ? "stdout" : "cout (risques de plantages)")
	     << endl;

	Writer*	writer	= new Writer (cStream);

	try
	{
		cout << "Creation of " << tasksCount << " infinite tasks ..." << endl;
		for (i = 0; i < tasksCount; i++)
			tasks.push_back (new InfiniteLoopThread (writer));
		cout << "Running tasks ..." << endl;
		for (i = 0; i < tasksCount; i++)
			tasks [i]->startTask ( );
		cout << "Sleeping 10 seconds in main thread ..." << endl;
		sleep (10);
		cout << "Canceling tasks ..." << endl;
		for (i = 0; i < tasksCount; i++)
			tasks [i]->cancel ( );
		cout << "Tasks canceled." << endl;
		for (i = 0; i < tasksCount; i++)
			tasks [i]->join ( );	// Calls delete
// Pb : on a peut être encore dans une pile thread->execute ( ).
// Le thread est joint, mais le thread existe peut être encore ...
//			delete tasks [i];
		cout << "Tasks joined." << endl;
		cout << "Sleeping 20 seconds ..." << endl;
		sleep (20);
		// join ne détruit plus le thread, on le fait ici :
		for (i = 0; i < tasksCount; i++)
			delete tasks [i];
		cout << "Program completed." << endl;
	}
	catch (const Exception& exc)
	{
		cout << "Exception caught : " << exc.getFullMessage ( ) << endl;
	}
	catch (...)
	{
		cout << "Unexpected exception caught." << endl;
	}
	tasks.clear ( );

	return 0;
}	// main


static int parseArgs (int argc, char* argv[])
{
	try
	{
		for (size_t i = 1; i < argc; i++)
		{
			if (0 == strcmp (argv [i], "-help"))
				return -1;
			else if (0 == strcmp (argv [i], "-cout"))
				cStream	= false;
			else if ((0 == strcmp (argv [i], "-tasks")) && (i < argc - 1))
				tasksCount	= NumericConversions::strToULong (argv [++i]);
			else
				return -1;
		}	// for (size_t i = 1; i < argc; i++)
	}
	catch (const Exception& exc)
	{
		cout << "Erreur : " << exc.getFullMessage ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cout << "Erreur non documentée." << endl;
		return -1;
	}

	return 0;
}	// parseArgs


static int syntax (const string& pgm)
{
	cout << pgm << " [-help][-tasks num][-cout]"
	     << endl
	     << "-help .................. : affiche ce message et quitte." << endl
	     << "-tasks num ............. : num tâches lancées dans des threads "
	     << "concurrents." << endl
	     << "-cout .................. : écritures dans le flux c++ cout, ce "
	     << "risque de provoquer des plantages." << endl
	     << endl
	     << endl;

	return -1;
}	// syntax
