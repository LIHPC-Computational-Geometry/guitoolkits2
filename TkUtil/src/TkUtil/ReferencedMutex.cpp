#include "TkUtil/ReferencedMutex.h"

#include <iostream>		// A SUPPRIMER
using namespace std;	// A SUPPRIMER


BEGIN_NAMESPACE_UTIL


ReferencedMutex::ReferencedMutex ( )
	: Mutex ( ), ReferencedObject (false)
{
}	// ReferencedMutex::ReferencedMutex


ReferencedMutex::ReferencedMutex (const ReferencedMutex&)
{
	assert (0 && "ReferencedMutex copy constructor is not allowed.");
}	// ReferencedMutex::ReferencedMutex


ReferencedMutex& ReferencedMutex::operator = (const ReferencedMutex&)
{
	assert (0 && "ReferencedMutex operator = is not allowed.");
	return *this;
}	// ReferencedMutex::operator =


ReferencedMutex::~ReferencedMutex ( )
{
	notifyObserversForDestruction ( );
//	unregisterReferences ( );
}	// ReferencedMutex::~ReferencedMutex


AutoReferencedMutex::AutoReferencedMutex (ReferencedMutex* mutex)
	: ObjectBase (false), _mutex (mutex)
{
	if (0 != _mutex)
	{
		_mutex->lock ( );
		registerObservable (_mutex, false);
	}
}	// AutoReferencedMutex::AutoReferencedMutex


AutoReferencedMutex::AutoReferencedMutex (const AutoReferencedMutex&)
{
	assert (0 && "AutoReferencedMutex copy constructor is not allowed.");
}	// AutoReferencedMutex::AutoReferencedMutex


AutoReferencedMutex& AutoReferencedMutex::operator = (const AutoReferencedMutex&)
{
	assert (0 && "AutoReferencedMutex operator = is not allowed.");
	return *this;
}	// AutoReferencedMutex::operator =


AutoReferencedMutex::~AutoReferencedMutex ( )
{
	unregisterReferences ( );

	if (0 != _mutex)
	{
		_mutex->unlock ( );
//		unregisterObservable (_mutex, false);
	}
}	// AutoReferencedMutex::AutoReferencedMutex


void AutoReferencedMutex::observableDeleted (ReferencedObject* object)
{
	ObjectBase::observableDeleted (object);

	ReferencedMutex*	mutex	= dynamic_cast<ReferencedMutex*>(object);
	if (mutex == _mutex)
		_mutex	= 0;
}	// AutoReferencedMutex::observableDeleted


END_NAMESPACE_UTIL
