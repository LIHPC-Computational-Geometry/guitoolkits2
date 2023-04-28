#include "TkUtil/Mutex.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <iostream>


USING_UTIL
USING_STD

BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


Mutex::Mutex (bool recursive)
	: _mutex ( ), _lockCount (0), _owner ((pthread_t)-1), _released (false)
{
	pthread_mutexattr_t		attr;
	int	ok	= pthread_mutexattr_init (&attr);
	if (0 == ok)
		ok	= pthread_mutexattr_settype (&attr,
			true==recursive ? PTHREAD_MUTEX_RECURSIVE:PTHREAD_MUTEX_ERRORCHECK);
	if (0 == ok)
		ok	= pthread_mutex_init (&_mutex, &attr);
	if (0 != ok)
	{
		UTF8String	err (charset);
		err << "Erreur de création d'un verrou. Code : " << (long)ok << ".";
		throw Exception (err);
	}	// if (0 != ok)
	ok	= pthread_mutexattr_destroy (&attr);
	if (0 != ok)
		cout << "Erreur de destruction des attributs d'un verrou. Code : "
		    << (long)ok << ".";
}	// Mutex::Mutex


Mutex::Mutex (const Mutex&)
{
	assert (0 && "Mutex copy constructor is not allowed.");
}	// Mutex::Mutex (const Mutex&)


Mutex& Mutex::operator = (const Mutex&)
{
	assert (0 && "Mutex operator = is not allowed.");
	return *this;
}	// Mutex::operator =


Mutex::~Mutex ( )
{
	if (true == isReleased ( ))
		return;

	release ( );
}	// Mutex::~Mutex


void Mutex::lock ( )
{	
	if (true == isReleased ( ))
		throw Exception (UTF8String ("Mutex::lock : mutex libéré.", charset));

	int	ok	= pthread_mutex_lock (&_mutex);
	if (0 != ok)
	{
		UTF8String	err (charset);
		err << "Erreur d'acquisition d'un verrou. Code : " << (long)ok << ".";
		throw Exception (err);
	}	// if (0 != ok)
	else
	{
		_lockCount++;
		if (1 == _lockCount)
			_owner	= pthread_self ( );
	}
}	// Mutex::lock


bool Mutex::tryLock ( )
{
	if (true == isReleased ( ))
		throw Exception (UTF8String("Mutex::tryLock : mutex libéré.", charset));

	int	ok	= pthread_mutex_trylock (&_mutex);
	if (0 != ok)
		return false;

	_lockCount++;
	if (1 == _lockCount)
		_owner	= pthread_self ( );

	return true;
}	// Mutex::tryLock


void Mutex::unlock ( )
{
	if ((true == isReleased ( )) || (0 == getLockCount ( )))
		return;

	if (1 == _lockCount)
		_owner	= (pthread_t)-1;
	_lockCount--;
	int	ok	= pthread_mutex_unlock (&_mutex);
	if (0 != ok)
	{
		_lockCount++;
		UTF8String	err (charset);
		err << "Erreur de libération d'un verrou. Code : " << (long)ok << ".";
		throw Exception (err);
	}	// if (0 != ok)
}	// Mutex::unlock


pthread_mutex_t Mutex::mutex ( ) const
{
	return _mutex;
}	// Mutex::mutex


void Mutex::release ( )
{
	if (true == _released)
		return;

	// Eviter une levée d'exception et s'assurer de la destruction effective
	// du mutex. La destruction d'une instance ayant un mutex verrouillé
	// (par plusieurs destructeurs successifs) avec des mutex récursifs
	// peut mener à ce type de situation.
	while (1 <= getLockCount ( ))
		unlock ( );

	_released	= true;
	int	ok	= pthread_mutex_destroy (&_mutex);
	if (0 != ok)
	{
		UTF8String	err (charset);
		err << "Erreur de destruction d'un verrou. Code : " << (long)ok << ".";
		throw Exception (err);
	}	// if (0 != ok)
}	// Mutex::release


// ============================================================================
//                              LA CLASSE AutoMutex
// ============================================================================


AutoMutex::AutoMutex (const AutoMutex&)
	: _mutex (0)
{
	assert (0 && "AutoMutex copy constructor is not allowed.");
}	// AutoMutex::AutoMutex (const AutoMutex&)


AutoMutex& AutoMutex::operator = (const AutoMutex&)
{
	assert (0 && "AutoMutex operator = is not allowed.");
	return *this;
}	// AutoMutex::operator =


END_NAMESPACE_UTIL
