#include "CorbaUtil/RemoteReferencedObjectImpl.h"
#include "CorbaUtil/ORBServices.h"
#include "CorbaUtil/ORBFacilities.h"
#include <TkUtil/UTF8String.h>
#include <TkUtil/NetworkData.h>
#include <TkUtil/Date.h>


#include <assert.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
USING_UTIL

static const Charset	charset ("àéèùô");


namespace CorbaFacilities
{

// ===========================================================================
//                           La classe RemoteObjectBaseImpl
// ===========================================================================


RemoteObjectBaseImpl::RemoteObjectBaseImpl ( )
	: POA_CorbaFacilities::RemoteObjectBase ( ),
	  _observables ( ), _id ( )
{
	_id	= createUniqueId ((size_t)this);
	ORBFacilities::getBidirPOA ( )->activate_object (this);
_remove_ref ( );	// 1.31.0
}	// RemoteObjectBaseImpl::RemoteObjectBaseImpl


RemoteObjectBaseImpl::RemoteObjectBaseImpl (const RemoteObjectBaseImpl&)
	: POA_CorbaFacilities::RemoteObjectBase ( ),
	  _observables ( ), _id ( )
{
	assert (0 && "RemoteObjectBaseImpl::RemoteObjectBaseImpl is not allowed.");
}	// RemoteObjectBaseImpl::RemoteObjectBaseImpl


RemoteObjectBaseImpl& RemoteObjectBaseImpl::operator = (
												const RemoteObjectBaseImpl&)
{
	assert (0 && "RemoteObjectBaseImpl::operator = is not allowed.");
	return *this;
}	// RemoteObjectBaseImpl::operator =


RemoteObjectBaseImpl::~RemoteObjectBaseImpl ( )
{
	unregisterRemoteReferences ( );
	PortableServer::ObjectId_var	id	=
						ORBFacilities::getBidirPOA ( )->servant_to_id (this);
	ORBFacilities::getBidirPOA ( )->deactivate_object (id);
}	// RemoteObjectBaseImpl::~RemoteObjectBaseImpl


char* RemoteObjectBaseImpl::getUniqueId ( )
{
	return CORBA::string_dup (_id.c_str ( ));
}	// RemoteObjectBaseImpl::getUniqueId


void RemoteObjectBaseImpl::remoteObservableModified (
					RemoteReferencedObject_ptr observable, CORBA::ULong kind)
{
}	// RemoteObjectBaseImpl::remoteObservableModified


void RemoteObjectBaseImpl::remoteObservableDeleted (
										RemoteReferencedObject_ptr observable)
{
	bool	completed	= false;
	if (false == CORBA::is_nil (observable))
	{
		while (false == completed)
		{
			completed	= true;

			for (vector<RemoteReferencedObject_var>::iterator	it =
			     _observables.begin ( ); _observables.end ( ) != it; it++)
			{
				if (true == compare<RemoteReferencedObject_ptr> (observable, *it))
				{
					_observables.erase (it);
					completed	= false;
					break;
				}
			}	// for (vector<RemoteReferencedObject_var>::iterator ...
		}	// while (false == completed)
	}	// if (false == CORBA::is_nil (observable))
}	// RemoteObjectBaseImpl::remoteObservableDeleted


void RemoteObjectBaseImpl::unregisterRemoteReferences ( )
{
	while (0 != _observables.size ( ))
	{
		vector<RemoteReferencedObject_var>::iterator	it	=
														_observables.begin ( );
		RemoteReferencedObject_var	object	= (*it);
		if (false == CORBA::is_nil (object))
		{
			RemoteObjectBase_ptr	obj	=
					CorbaFacilities::RemoteObjectBase::_duplicate (_this ( ));
			object->removeRemoteObserver (obj);
		}	// if (false == CORBA::is_nil (object))
		_observables.erase (it);
	}	// while (0 != _observables.size ( ))
}	// RemoteObjectBaseImpl::unregisterRemoteReferences


void RemoteObjectBaseImpl::registerRemoteObservable (
									RemoteReferencedObject_ptr& observable)
{
	if (false == CORBA::is_nil (observable))
	{
		RemoteObjectBase_ptr	object	=
					CorbaFacilities::RemoteObjectBase::_duplicate (_this ( ));
		observable->addRemoteObserver (object);
		_observables.push_back (
					RemoteReferencedObject::_duplicate (observable));
	}	// if (false == CORBA::is_nil (observable))
}	// RemoteObjectBaseImpl::registerRemoteObservable


void RemoteObjectBaseImpl::unregisterRemoteObservable (
										RemoteReferencedObject_ptr& observable)
{
	if (false == CORBA::is_nil (observable))
	{
		for (vector<RemoteReferencedObject_var>::iterator	it =
		     _observables.begin ( ); _observables.end ( ) != it; it++)
		{
			if (true == compare<RemoteReferencedObject_ptr> (observable, *it))
			{
				_observables.erase (it);
				break;
			}
		}	// for (vector<RemoteReferencedObject_var>::iterator ...

// V 1.31.0 : commentaires
//		RemoteObjectBase_ptr	obj	=
//				CorbaFacilities::RemoteObjectBase::_duplicate (_this ( ));
//		observable->removeRemoteObserver (obj);
	}	// if (false == CORBA::is_nil (observable))
}	// RemoteObjectBaseImpl::unregisterRemoteObservable


string RemoteObjectBaseImpl::createUniqueId (size_t ptr)
{
	UTF8String	id (charset);
//	NetworkData		host;
	const NetworkData&		host    = NetworkData::getLocalHost ( );
	Date			date;

	id << host.getInetAddress ( ) << "_" << (unsigned long)getpid ( )
	   << "_" << date.getDate ( ) << "_" << (unsigned long)ptr;
	return id.ascii ( );
}	// RemoteObjectBaseImpl::createUniqueId


// ===========================================================================
//                     La classe RemoteReferencedObjectImpl
// ===========================================================================

RemoteReferencedObjectImpl::RemoteReferencedObjectImpl ( )
	: POA_CorbaFacilities::RemoteReferencedObject ( ),
	  RemoteObjectBaseImpl ( ),
	  _observers ( )
{
}	// RemoteReferencedObjectImpl::RemoteReferencedObjectImpl


RemoteReferencedObjectImpl::RemoteReferencedObjectImpl (
											const RemoteReferencedObjectImpl&)
	: POA_CorbaFacilities::RemoteReferencedObject ( ),
	  RemoteObjectBaseImpl ( ),
	  _observers ( )
{
	assert (0 && "RemoteReferencedObjectImpl::RemoteReferencedObjectImplRemoteObjectBaseImpl is not allowed.");
}	// RemoteReferencedObjectImpl::RemoteReferencedObjectImpl


RemoteReferencedObjectImpl& RemoteReferencedObjectImpl::operator = (
											const RemoteReferencedObjectImpl&)
{
	assert (0 && "RemoteReferencedObjectImpl::operator = is not allowed.");
	return *this;
}	// RemoteReferencedObjectImpl::operator =


RemoteReferencedObjectImpl::~RemoteReferencedObjectImpl ( )
{
	notifyRemoteObserversForDestruction ( );
}	// RemoteReferencedObjectImpl::~RemoteReferencedObjectImpl


void RemoteReferencedObjectImpl::addRemoteObserver (
											RemoteObjectBase_ptr& observable)
{
	_observers.push_back (
			CorbaFacilities::RemoteObjectBase::_duplicate (observable));
}	// RemoteReferencedObjectImpl::addRemoteObserver


void RemoteReferencedObjectImpl::removeRemoteObserver (
											RemoteObjectBase_ptr& observable)
{
	for (vector<RemoteObjectBase_var>::iterator it = _observers.begin ( );
	     _observers.end ( ) != it; it++)
	{
		if (true == compare<RemoteObjectBase_ptr> (observable, *it))
		{
			_observers.erase (it);
			break;
		}	// if (true == compare (observable, *it))
	}	// for (vector<RemoteObjectBase_var>::iterator it = ...
}	// RemoteReferencedObjectImpl::removeRemoteObserver


void RemoteReferencedObjectImpl::notifyRemoteObserversForModification (
															CORBA::ULong kind)
{
	if (0 != _observers.size ( ))
	{
		CorbaFacilities::RemoteReferencedObject_ptr	me	=
					POA_CorbaFacilities::RemoteReferencedObject::_this ( );
		RemoteObserverModifiedCallback*	callback	=
			new RemoteObserverModifiedCallback (me, _observers, kind);
		callback->start ( );
	}	// if (0 != _observers.size ( ))
}	// RemoteReferencedObjectImpl::notifyRemoteObserversForModification


void RemoteReferencedObjectImpl::notifyRemoteObserversForDestruction ( )
{
	if (0 != _observers.size ( ))
	{
		CorbaFacilities::RemoteReferencedObject_ptr	me	=
					POA_CorbaFacilities::RemoteReferencedObject::_this ( );
		vector<RemoteObjectBase_var>	list (_observers);
		_observers.clear ( );
		RemoteObserverDeletedCallback*	callback	=
					new RemoteObserverDeletedCallback (me, list);
		callback->join (0);
	}	// if (0 != _observers.size ( ))
}	// RemoteReferencedObjectImpl::notifyRemoteObserversForDestruction ( )


// ===========================================================================
//       La classe RemoteReferencedObjectImpl::RemoteObserverCallback
// ===========================================================================


RemoteReferencedObjectImpl::RemoteObserverCallback::RemoteObserverCallback (
					RemoteReferencedObject_ptr observable,
					const std::vector<RemoteObjectBase_var>& list)
#ifdef OMNIORB
	: omni_thread ( ),
#endif	// OMNIORB
	  _observable (0), _observersList ( )
{
	_observable	=
			CorbaFacilities::RemoteReferencedObject::_duplicate (observable);
	for (vector<RemoteObjectBase_var>::const_iterator it = list.begin( );
	     list.end ( ) != it; it++)
	{
		RemoteObjectBase_ptr	observer	=
				CorbaFacilities::RemoteObjectBase::_duplicate ((*it));
		_observersList.push_back (observer);
	}	// for (std::vector<RemoteObjectBase_var>::const_iterator it ...
}	// RemoteObserverCallback::RemoteObserverCallback


// ===========================================================================
//    La classe RemoteReferencedObjectImpl::RemoteObserverModifiedCallback
// ===========================================================================

void RemoteReferencedObjectImpl::RemoteObserverModifiedCallback::run (void*)
{
	vector<RemoteObjectBase_var>&	observers	= getObserversList ( );
	for (vector<RemoteObjectBase_var>::iterator it = observers.begin ( );
	      observers.end ( ) != it; it++)
				(*it)->remoteObservableModified (
												getObservable ( ), getKind ( ));
}	// RemoteObserverModifiedCallback::run


// ===========================================================================
//    La classe RemoteReferencedObjectImpl::RemoteObserverDeletedCallback
// ===========================================================================

#ifdef OMNIORB
void* RemoteReferencedObjectImpl::RemoteObserverDeletedCallback::run_undetached (void*)
{
	vector<RemoteObjectBase_var>&	observers	= getObserversList ( );
	for (vector<RemoteObjectBase_var>::iterator it = observers.begin ( );
		 observers.end ( ) != it; it++)
	{
		try
		{
			(*it)->remoteObservableDeleted (getObservable ( ));
		}
		catch (...)
		{	// Objet déjà détruit côté client.
		}
	}

	return 0;
}	// RemoteObserverModifiedCallback::run
#else	// OMNIORB
#	error "Classe de base de threads non définie pour cet ORB"
#endif	// OMNIORB


// ===========================================================================
//                     La classe RemoteReferencedObjectImpl
// ===========================================================================


RemoteReferencedNamedObjectImpl::RemoteReferencedNamedObjectImpl (
															const string& name)
	: POA_CorbaFacilities::RemoteReferencedNamedObject ( ),
	  RemoteReferencedObjectImpl ( ), _name (name)
{
}	// RemoteReferencedNamedObjectImpl::RemoteReferencedNamedObjectImpl


RemoteReferencedNamedObjectImpl::RemoteReferencedNamedObjectImpl (
									const RemoteReferencedNamedObjectImpl& copy)
	: POA_CorbaFacilities::RemoteReferencedNamedObject ( ),
	  RemoteReferencedObjectImpl ( ), _name (copy._name)
{
	assert (0 && "RemoteReferencedNamedObjectImpl::RemoteReferencedNamedObjectImpl is not allowed.");
}	// RemoteReferencedNamedObjectImpl::RemoteReferencedNamedObjectImpl


RemoteReferencedNamedObjectImpl& RemoteReferencedNamedObjectImpl::operator = (
										const RemoteReferencedNamedObjectImpl&)
{
	assert (0 && "RemoteReferencedNamedObjectImpl::operator = is not allowed.");
	return *this;
}	// RemoteReferencedNamedObjectImpl::operator =


RemoteReferencedNamedObjectImpl::~RemoteReferencedNamedObjectImpl ( )
{
}	// RemoteReferencedNamedObjectImpl::~RemoteReferencedNamedObjectImpl


char* RemoteReferencedNamedObjectImpl::getRemoteName ( )
{
	return CORBA::string_dup (_name.c_str ( ));
}	// RemoteReferencedNamedObjectImpl::getRemoteName


void RemoteReferencedNamedObjectImpl::setRemoteName (const char* name)
{
	BEGIN_CORBA_TRY_CATCH_BLOCK

	_name	= name;

	COMPLETE_CORBA_TRY_CATCH_BLOCK
	THROW_APPLICATION_EXCEPTION_IF_ERROR
}	// RemoteReferencedNamedObjectImpl::setRemoteName


}	// namespace CorbaFacilities

