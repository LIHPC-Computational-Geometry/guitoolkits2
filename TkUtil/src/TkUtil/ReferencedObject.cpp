#include "TkUtil/ReferencedObject.h"
#include "TkUtil/ReferencedMutex.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <iostream>		// A SUPPRIMER


USING_STD

static const IN_UTIL Charset	charset ("àéèùô");

#ifdef MULTITHREADED_APPLICATION

#define LOCK_OBJECT_BASE                                                     \
	AutoMutex 	autoMutex (getMutex ( ));

#else	// MULTITHREADED_APPLICATION

#define LOCK_OBJECT_BASE

#endif	// MULTITHREADED_APPLICATION


BEGIN_NAMESPACE_UTIL


// ============================================================================
//                            LA CLASSE ObjectBase
// ============================================================================


ObjectBase::ObjectBase (bool hasMutex)
	: _observables ( )
#ifdef MULTITHREADED_APPLICATION
	, _mutex (true == hasMutex ? new ReferencedMutex ( ) : 0)
#endif	// MULTITHREADED_APPLICATION
{
}	// ObjectBase::ObjectBase


ObjectBase::ObjectBase (const ObjectBase&)
	: _observables ( )
{
	assert (0 && "ObjectBase::ObjectBase (const ObjectBase&) is not allowed.");
}	// ObjectBase::ObjectBase (const ObjectBase&)


ObjectBase& ObjectBase::operator = (const ObjectBase&)
{
	assert (0 && "ObjectBase::operator = (const ObjectBase&) is not allowed.");
	return *this;
}	// ObjectBase::operator =


ObjectBase::~ObjectBase ( )
{
	unregisterReferences ( );
#ifdef MULTITHREADED_APPLICATION
	delete _mutex;
#endif	// MULTITHREADED_APPLICATION
}	// ObjectBase::~ObjectBase


void ObjectBase::observableModified (ReferencedObject*, unsigned long)
{
}	// ObjectBase::observableModified


void ObjectBase::observableDeleted (ReferencedObject* object)
{
	LOCK_OBJECT_BASE

	bool	completed	= false;

	while (false == completed)
	{
		completed	= true;

		for (multimap<ReferencedObject*, bool>::iterator ito = _observables.begin ( ); _observables.end ( ) != ito; ito++)
			if (object == (*ito).first)
			{
				_observables.erase (ito);
				completed	= false;
				break;
			}	// if (object == *ito)
	}	// while (false == completed)

#ifndef NDEBUG
//	for (map<ReferencedObject*, bool>::iterator it = _observables.begin ( );
//	     _observables.end ( ) != it; it++)
//		assert (object != (*it).first);
#endif
}	// ObjectBase::observableDeleted


void ObjectBase::unregisterReferences ( )
{
	LOCK_OBJECT_BASE

	while (0 != _observables.size ( ))
	{
		multimap<ReferencedObject*, bool>::iterator	ito	= _observables.begin( );
		ReferencedObject*	object	= (*ito).first;
		bool				block	= (*ito).second;
		if (0 != object)
			object->removeObserver (this, block);
		_observables.erase (ito);
	}	// while (0 != _observables.size ( ))
}	// ObjectBase::unregisterReferences


void ObjectBase::registerObservable (ReferencedObject* object, bool blockDestruction)
{
	LOCK_OBJECT_BASE

	if (0 != object)
	{
		object->addObserver (this, blockDestruction);
		_observables.insert (pair <ReferencedObject*, bool>(object, blockDestruction));
	}	// if (0 != object)
}	// ObjectBase::registerObservable


void ObjectBase::unregisterObservable (ReferencedObject* object, bool blockDestruction)
{
	LOCK_OBJECT_BASE

	if (0 != object)
	{
		for (multimap<ReferencedObject*, bool>::iterator it = _observables.begin ( ); _observables.end ( ) != it; it++)
		{
			if (((*it).first == object) && (blockDestruction == (*it).second))
			{
				_observables.erase (it);
				break;
			}
		}
		object->removeObserver (this, blockDestruction);
	}	// if (0 != object)
}	// ObjectBase::unregisterObservable


bool ObjectBase::isObservableRegistered (ReferencedObject* object, bool blockDestruction) const
{
	LOCK_OBJECT_BASE

	for (multimap<ReferencedObject*, bool>::const_iterator it = _observables.begin ( ); _observables.end ( ) != it; it++)
		if (((*it).first == object) && (blockDestruction == (*it).second))
		{
			return true;
		}

	return false;
}	// ObjectBase::isObservableRegistered


size_t ObjectBase::getObservableCount ( ) const
{
	return _observables.size ( );
}	// ObjectBase::getObservableCount


const vector<ReferencedObject*> ObjectBase::getObservables ( ) const
{
	vector<ReferencedObject*>	observables;

	LOCK_OBJECT_BASE

	for (multimap<ReferencedObject*, bool>::const_iterator it = _observables.begin ( ); _observables.end ( ) != it; it++)
		observables.push_back ((*it).first);

	return observables;
}	// ObjectBase::getObservables ( )


vector<ReferencedObject*> ObjectBase::getObservables ( )
{
	vector<ReferencedObject*>	observables;

	LOCK_OBJECT_BASE

	for (multimap<ReferencedObject*, bool>::iterator it = _observables.begin( ); _observables.end ( ) != it; it++)
		observables.push_back ((*it).first);

	return observables;
}	// ObjectBase::getObservables ( )


void ObjectBase::emergencyCleanup ( )
{
	while (0 != _observables.size ( ))
	{
		multimap<ReferencedObject*, bool>::iterator	ito	= _observables.begin( );
		ReferencedObject*	object	= (*ito).first;
		// On évite au objets observés de se détruire, on est en situation peu
		// contrôlée :
		bool	block	= false;	// (*ito).second;
		if (0 != object)
			object->removeObserver (this, block);
		_observables.erase (ito);
	}	// while (0 != _observables.size ( ))
}


// ============================================================================
//                         LA CLASSE ReferencedObject
// ============================================================================


ReferencedObject::ReferencedObject (bool hasMutex)
	: ObjectBase (hasMutex), _observers ( )
{
}	// ReferencedObject::ReferencedObject ( )


ReferencedObject::ReferencedObject (const ReferencedObject&)
	: ObjectBase (false), _observers ( )
{
	assert (0 && "ReferencedObject::ReferencedObject (const ReferencedObject&) is not allowed.");
}	// ReferencedObject::ReferencedObject ( )


ReferencedObject& ReferencedObject::operator = (const ReferencedObject&)
{
	assert (0 && "ReferencedObject::operator = is not allowed.");
	return *this;
}	// ReferencedObject::operator =


ReferencedObject::~ReferencedObject ( )
{
//	assert (true == allowsDestruction ( ));

	notifyObserversForDestruction ( );
}	// ReferencedObject::~ReferencedObject


void ReferencedObject::addObserver (ObjectBase* object, bool blockDestruction)
{
	LOCK_OBJECT_BASE

	_observers.insert (pair <ObjectBase*, bool>(object, blockDestruction));
}	// ReferencedObject::addObserver


void ReferencedObject::removeObserver(ObjectBase* object, bool blockDestruction)
{
	// CP : pthread_cleanup_push et pthread_cleanup_pop sont des macros qui
	// insèrent des { et }, d'où les lignes de code qui suivent ...
#ifdef MULTITHREADED_APPLICATION
	if (0 != getMutex ( ))
	{
//		pthread_cleanup_push (	// v 2.34.0
//			(void (*)(void*))pthread_mutex_unlock, &(getMutex ( )->mutex ( )));
		pthread_mutex_t	mutex	= getMutex ( )->mutex ( );	// v 2.34.0
		pthread_cleanup_push ((void (*)(void*))pthread_mutex_unlock, &mutex);	// v 2.34.0
		getMutex ( )->lock ( );

		for (multimap<ObjectBase*, bool>::iterator it = _observers.begin ( );
		     _observers.end ( ) != it; it++)
		{
			if (((*it).first == object) && ((*it).second == blockDestruction))
			{
				_observers.erase (it);
				break;
			}	// if (((*it).first == object) && ...
		}	// for (multimap<ObjectBase*, bool>::iterator it = ...

		getMutex ( )->unlock ( );
		pthread_cleanup_pop (0);
	}	// if (0 != getMutex ( ))
	else
	{
		for (multimap<ObjectBase*, bool>::iterator it = _observers.begin ( );
		     _observers.end ( ) != it; it++)
		{
			if (((*it).first == object) && ((*it).second == blockDestruction))
			{
				_observers.erase (it);
				break;
			}	// if (((*it).first == object) && ...
		}	// for (multimap<ObjectBase*, bool>::iterator it = ...
	}	// else if (0 != getMutex ( ))
#else	// MULTITHREADED_APPLICATION
	for (multimap<ObjectBase*, bool>::iterator it = _observers.begin ( );
	     _observers.end ( ) != it; it++)
	{
		if (((*it).first == object) && ((*it).second == blockDestruction))
		{
			_observers.erase (it);
			break;
		}	// if (((*it).first == object) && ...
	}	// for (multimap<ObjectBase*, bool>::iterator it = ...
#endif	// MULTITHREADED_APPLICATION

	// if (true == blockDestruction) : eviter que delete this soit appele a
	// plusieurs reprises ...
	if ((true == blockDestruction) && (true == allowsDestruction ( )))
		delete this;
}	// ReferencedObject::removeObserver


size_t ReferencedObject::getObserverCount ( ) const
{
	return _observers.size ( );
}	// ReferencedObject::getObserverCount


vector<ObjectBase*> ReferencedObject::getObservers ( ) const
{
	LOCK_OBJECT_BASE

	vector<ObjectBase*>	observers;

	for (multimap<ObjectBase*, bool>::const_iterator it = _observers.begin ( );
	     _observers.end ( ) != it; it++)
		observers.push_back ((*it).first);

	return observers;
}	// ReferencedObject::getObservers


void ReferencedObject::emergencyCleanup ( )
{
	ObjectBase::emergencyCleanup ( );

	multimap<ObjectBase*, bool>	copy	= _observers;	// CP v 2.9.0
	for (multimap<ObjectBase*, bool>::iterator	it	= copy.begin ( );
	     copy.end ( ) != it; it++)
		if (0 != (*it).first)
			((*it).first)->observableModified (this, UTIL_UNAVAILABLE_EVENT);
}	// ReferencedObject::emergencyCleanup


void ReferencedObject::notifyObserversForModification (unsigned long event)
{
	LOCK_OBJECT_BASE

	multimap<ObjectBase*, bool>	copy	= _observers;	// CP v 2.9.0
	for (multimap<ObjectBase*, bool>::iterator	it	= copy.begin ( );
	     copy.end ( ) != it; it++)
		if (0 != (*it).first)
			((*it).first)->observableModified (this, event);
/*	v < 2.9.0
	Pb : si un observateur fait un unregisterObservable alors it devient faux
	et ça plante, d'où l'utilité de travailler sur une copie ...
	for (multimap<ObjectBase*, bool>::iterator	it	= _observers.begin ( );
	     _observers.end ( ) != it; it++)
		if (0 != (*it).first)
			((*it).first)->observableModified (this, event); */
}	// ReferencedObject::notifyObserversForModification


void ReferencedObject::notifyObserversForDestruction ( )
{
	LOCK_OBJECT_BASE

	while (0 != _observers.size ( ))
	{
		multimap<ObjectBase*, bool>::iterator	ito	= _observers.begin ( );
		ObjectBase*	object	= (*ito).first;
		_observers.erase (ito);
		if (0 != object)
			object->observableDeleted (this);
	}
}	// ReferencedObject::notifyObserversForDestruction


bool ReferencedObject::allowsDestruction ( ) const
{
	LOCK_OBJECT_BASE

	for (multimap<ObjectBase*, bool>::const_iterator
				ito	= _observers.begin ( ); _observers.end ( ) != ito; ito++)
			if (true == (*ito).second)
				return false;

	return true;
}	// ReferencedObject::allowsDestruction


// ============================================================================
//                       LA CLASSE ReferencedNamedObject
// ============================================================================

ReferencedNamedObject::ReferencedNamedObject (const string& name, bool hasMutex)
	: ReferencedObject (hasMutex), _name (name), _uniqueName (name)
{
}	// ReferencedNamedObject::ReferencedNamedObject



ReferencedNamedObject::ReferencedNamedObject (const ReferencedNamedObject&)
	: ReferencedObject ( ), _name ("INTERNAL_ERROR"), _uniqueName ( )
{
	assert (0 && "ReferencedNamedObject::ReferencedNamedObject (const ReferencedNamedObject&) is not allowed.");
}	// ReferencedNamedObject::ReferencedNamedObject ( )


ReferencedNamedObject& ReferencedNamedObject::operator = (const ReferencedNamedObject&)
{
	assert (0 && "ReferencedNamedObject::operator = is not allowed.");
	return *this;
}	// ReferencedNamedObject::operator =


ReferencedNamedObject::~ReferencedNamedObject ( )
{
}	// ReferencedNamedObject::~ReferencedNamedObject


void ReferencedNamedObject::setName (const string& name)
{
	LOCK_OBJECT_BASE

	if (name != _name)
	{
		_name	= name;
		notifyObserversForModification (UTIL_NAME_MODIFIED);
	}
}	// ReferencedNamedObject::setName


void ReferencedNamedObject::setUniqueName (const string& name)
{
	LOCK_OBJECT_BASE

	if (name != _uniqueName)
	{
		_uniqueName	= name;
		notifyObserversForModification (UTIL_NAME_MODIFIED);
	}
}	// ReferencedNamedObject::setUniqueName


// ============================================================================
//                      LA CLASSE ReferencedObjectManager
// ============================================================================


ReferencedObjectManager*	ReferencedObjectManager::_instance	= 0;


ReferencedObjectManager::ReferencedObjectManager ( )
	: ReferencedObject (true)	// v 5.11.5 : construction avec un mutex (défaut : sans mutex)
{
	assert (0 == _instance);
	_instance	= this;
}	// ReferencedObjectManager::ReferencedObjectManager


ReferencedObjectManager::ReferencedObjectManager (const ReferencedObjectManager&)
	: ReferencedObject ( )
{	
	assert (0 && "ReferencedObjectManager copy constructor is not allowed.");
}	// ReferencedObjectManager::ReferencedObjectManager


ReferencedObjectManager& ReferencedObjectManager::operator = (const ReferencedObjectManager&)
{	
	assert (0 && "ReferencedObjectManager operator = is not allowed.");	
	return *this;
}	// ReferencedObjectManager::ReferencedObjectManager


ReferencedObjectManager::~ReferencedObjectManager ( )
{
	notifyObserversForDestruction ( );

	_instance	= 0;

	unregisterReferences () ;
}	// ReferencedObjectManager::~ReferencedObjectManager


ReferencedObjectManager& ReferencedObjectManager::getManagerInstance ( )
{
	if (0 == _instance)
		throw Exception (UTF8String ("ReferencedObjectManager::getManagerInstance : gestionnaire non initialisé.", charset));

	return *_instance;
}	// ReferencedObjectManager::getManagerInstance


void ReferencedObjectManager::registerObject (ReferencedNamedObject* object)
{
	LOCK_INSTANCE

	if (0 == object)
		throw Exception (UTF8String ("ReferencedObjectManager::registerObject : objet nul", charset));
	const string	uniqueName	= object->getUniqueName ( );
	if (0 == uniqueName.length ( ))
		throw Exception (UTF8String ("Impossibilité d'enregistrer un objet de nom unique nul auprès du service de gestion d'objets.", charset));

	if (false == isObservableRegistered (object, false))
	{
		// Vérifier l'unicité du nom :
		vector<ReferencedObject*>	objects	= getObservables ( );

		for (vector<ReferencedObject*>::iterator it = objects.begin ( );
		     objects.end ( ) != it; it++)
		{
			ReferencedNamedObject*	namedObject	= dynamic_cast<ReferencedNamedObject*>(*it);
			assert (0 != namedObject);
			if (uniqueName == namedObject->getUniqueName ( ))
			{
				UTF8String	message (charset);
				message << "Un objet de nom unique " << uniqueName
				        << " est déjà référencé auprès du service de gestion d'objets.";
				throw Exception (message);
			}	// if (uniqueName == namedObject->getUniqueName ( ))
		}	// for (vector<ReferencedObject*>::iterator it = objects.begin ( );

		// C'est OK, on l'enregistre auprès du service :
		registerObservable (object, false);
	}
}	// ReferencedObjectManager::registerObject


void ReferencedObjectManager::unregisterObject (ReferencedNamedObject* object)
{
	LOCK_INSTANCE

	if (0 == object)
		throw Exception (UTF8String ("ReferencedObjectManager::unregisterObject : objet nul", charset));

	if (true == isObservableRegistered (object, false))
		unregisterObservable (object, false);
}	// ReferencedObjectManager::unregisterObject


ReferencedNamedObject* ReferencedObjectManager::getInstance (const string& name)
{
	LOCK_INSTANCE

	vector<ReferencedObject*>	objects	= getObservables ( );

	for (vector<ReferencedObject*>::iterator it = objects.begin ( );
	     objects.end ( ) != it; it++)
	{
		ReferencedNamedObject*	namedObject	= dynamic_cast<ReferencedNamedObject*>(*it);
		assert (0 != namedObject);
		if (name == namedObject->getUniqueName ( ))
			return namedObject;
	}

	UTF8String	message (charset);
	message << "Objet " << (0 == name.length ( ) ? "\"sans nom\"" : name)
	        << " non référencé auprès du service de gestion d'objets.";
	throw Exception (message);
}	// ReferencedObjectManager::getInstance


ReferencedObjectManager& getObjectManagerInstance ( )
{
	return ReferencedObjectManager::getManagerInstance ( );
}	// getObjectManagerInstance


END_NAMESPACE_UTIL
