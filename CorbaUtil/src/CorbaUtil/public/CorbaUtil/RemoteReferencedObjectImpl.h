#ifndef REMOTE_REFERENCED_OBJET_IMPL_H
#define REMOTE_REFERENCED_OBJET_IMPL_H

#include "RemoteReferencedObject.hh"

#include <assert.h>
#include <string>
#include <vector>


namespace CorbaFacilities
{

/**
 * Implémentation de la classe de base RemoteObjectBase d'un système
 * observable/observateurs Corba.
 * @warning		L'appel de <I>remoteObservableModified</I> risque d'être
 *				effectué dans un thread autre que le thread principal.
 *				Il convient de faire très attention aux bibliothèques
 *				utilisées par ces fonctions callbacks, certaines n'étant
 *				pas multithread (ex : Xlib).
 */
class RemoteObjectBaseImpl : public POA_CorbaFacilities::RemoteObjectBase
{
	public :

	/**
	 * Constructeur. RAS.
	 */
	RemoteObjectBaseImpl ( );

	/**
	 * Constructeur.
	 */
	virtual ~RemoteObjectBaseImpl ( );

	/**
	 * @return		Un identifiant unique sur le réseau.
	 */
	virtual char* getUniqueId ( );

	/**
	 * Appellé lorsque l'objet observé est modifié.
	 * @param		Object modifié
	 * @param		Nature de la modification
	 */
	virtual void remoteObservableModified (
					RemoteReferencedObject_ptr observable, CORBA::ULong kind);

	/**
	 * Appellé lorsque l'objet appelé est en cours de destruction.
	 * @param		Object en cours de destruction
	 */
	virtual void remoteObservableDeleted(RemoteReferencedObject_ptr observable);

	/**
	 * Se désenregistre auprès de tous les objets qui le référencent.
	 */
	virtual void unregisterRemoteReferences ( );

	/**
	 * S'enregistre auprès de l'objet transmis en argument pour être averti
	 * de toute modification le concernant.
	 * @param		Objet à observer
	 * @see			unregisterRemoteObservable
	 * @see			remoteObservableModified
	 * @see			remoteObservableDeleted
	 */
	virtual void registerRemoteObservable (
		RemoteReferencedObject_ptr& observable);

	/**
	 * Se désenregistre auprès de l'objet transmis en argument.
	 * de toute modification le concernant.
	 * @param		Objet à ne plus observer
	 * @see			remoteObservableModified
	 * @see			remoteObservableDeleted
	 */
	virtual void unregisterRemoteObservable (
									RemoteReferencedObject_ptr& observable);

	/**
	 * Créé un identifiant unique sur le réseau pour le pointeur transmis
	 * en argument.
	 */
	static std::string createUniqueId (size_t ptr);


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	RemoteObjectBaseImpl (const RemoteObjectBaseImpl&);
	RemoteObjectBaseImpl& operator = (const RemoteObjectBaseImpl&);

	/** L'identifiant unique de l'objet. */
	std::string											_id;

	/** La liste des objets observés. */
	std::vector<RemoteReferencedObject_var>				_observables;

	/** Le compteur d'identifiants uniques. */
	static CORBA::ULong									_currentId;
};	// class RemoteObjectBaseImpl


/**
 * return		true si les deux objets ont le même identifiant unique,
 *				false dans le cas contraire.
 */
template <class T_ptr>inline bool compare (const T_ptr& l, const T_ptr& r)
{
	CORBA::String_var	lid	= l->getUniqueId ( );
	CORBA::String_var	rid	= r->getUniqueId ( );
	return 0 == strcmp (lid, rid) ? true : false;
}	// compare


/**
 * Implémentation d'une classe observable Corba.
 */
class RemoteReferencedObjectImpl :
					public POA_CorbaFacilities::RemoteReferencedObject,
					public RemoteObjectBaseImpl
{
	public :

	/**
	 * Constructeur. RAS.
	 */
	RemoteReferencedObjectImpl ( );

	/**
	 * Destructeur. Informe ses observateurs de sa destruction.
	 */
	virtual ~RemoteReferencedObjectImpl ( );

	/**
	 * Recense l'objet transmis en argument pour l'informer de toute
	 * modification le concernant.
	 * @param		Observateur à recenser
	 * @see			removeRemoteObserver
	 */
	virtual void addRemoteObserver(RemoteObjectBase_ptr& observable);

	/**
	 * Désenregistre une référence sur l'objet transmis en argument.
	 * Se détruit s'il n'a plus aucune référence sur d'autres objets
	 * bloquant sa destruction.
	 * @param		Observateur à déréférencer
	 * @param		drapeau de blocage de l'autodestruction
	 * @see			removeRemoteObserver
	 */
	virtual void removeRemoteObserver (RemoteObjectBase_ptr& observable);


	protected :

	/**
	 * <P>Classe permettant d'invoquer une méthode des observateurs dans un
	 * autre thread afin d'éviter des dead-locks entre le serveur et ses
	 * clients.</P>
	 * 
	 * <P>La méthode <I>run</I> est à surcharger afin d'invoquer la bonne
	 * fonction membre des observateurs.</P>
	 */
	class RemoteObserverCallback :
#ifdef OMNIORB
												public omni_thread
#else	// OMNIORB
#	error "Classe de base de threads non définie pour cet ORB"
#endif	// OMNIORB
	{
		public :

		/**
		 * Constructeur : RAS.
		 * @param		Observable à l'origine de l'appel des observateurs.
		 * @param		Liste des observateurs à appeler.
		 */
		RemoteObserverCallback (RemoteReferencedObject_ptr observable,
	                        const std::vector<RemoteObjectBase_var>& list);

		/**
		 * Destructeur : RAS.
		 */
		virtual ~RemoteObserverCallback ( )
		{ }


		protected :

		/**
		 * @return		Observable à l'origine de l'appel des observateurs.
		 */
		RemoteReferencedObject_var& getObservable ( )
		{ return _observable; }

		/**
		 * @return		La liste des observateurs à appeler
		 */
		std::vector<RemoteObjectBase_var>& getObserversList ( )
		{ return _observersList; }


		private :

		/**
		 * Constructeurs de copie et opérateur = : interdits.
		 */
		RemoteObserverCallback (const RemoteReferencedObject&)
#ifdef OMNIORB
			: omni_thread ( )
#endif	// OMNIORB
		{
			assert(0&&"RemoteObserverCallback copy constructor is not allowed");
		}
		RemoteObserverCallback& operator = (const RemoteObserverCallback&)
		{
			assert(0&&"RemoteObserverCallback operator = is not allowed");
			return *this;
		}

		 /** Observable à l'origine de l'appel des observateurs.  */
		RemoteReferencedObject_var					_observable;

		/** La liste des observateurs à appeler */
		std::vector<RemoteObjectBase_var>			_observersList;
	};	// class RemoteObserverCallback :

	/**
	 * Classe permettant d'appeler les observateurs en cas de modification d'un
	 * observable.
	 */
	class RemoteObserverModifiedCallback : public RemoteObserverCallback
	{
		public :

		/**
		 * Constructeur : RAS.
		 * @param		Observable à l'origine de l'appel des observateurs.
		 * @param		Liste des observateurs à appeler.
		 * @param		Nature de la modification.
		 */
		RemoteObserverModifiedCallback (
					RemoteReferencedObject_ptr observable,
					const std::vector<RemoteObjectBase_var>& list,
					CORBA::ULong kind)
			: RemoteObserverCallback (observable, list), _kind (kind)
		{ }

		/**
		 * Destructeur : RAS.
		 */
		virtual ~RemoteObserverModifiedCallback ( )
		{ }


		/**
		 * L'appel des observateurs.
		 */
		virtual void run (void*);


		protected :

		/**
		 * @return		La nature de la modification.
		 */
		CORBA::ULong getKind ( ) const
		{ return _kind; }


		private :

		/**
		 * Constructeurs de copie et opérateur = : interdits.
		 */
		RemoteObserverModifiedCallback (const RemoteObserverModifiedCallback&)
			: RemoteObserverCallback(0,std::vector<RemoteObjectBase_var> ( )),
			  _kind (0)
		{
			assert(0&&"RemoteObserverModifiedCallback copy constructor is not allowed");
		}
		RemoteObserverModifiedCallback& operator = (
									const RemoteObserverModifiedCallback&)
		{
			assert(0&&"RemoteObserverModifiedCallback operator = is not allowed");
			return *this;
		}


		 /** La nature de la modification. */
		CORBA::ULong				_kind;
	};	// class RemoteObserverModifiedCallback

	/**
	 * Classe permettant d'appeler les observateurs en cas de destruction d'un
	 * observable.
	 * @warning		Il est nécessaire de faire un join sur ce thread avant
	 *				d'achever la destruction de l'observable.
	 */
	class RemoteObserverDeletedCallback : public RemoteObserverCallback
	{
		public :

		/**
		 * Constructeur : lance l'exécution du traitement.
		 * @param		Observable à l'origine de l'appel des observateurs.
		 * @param		Liste des observateurs à appeler.
		 */
		RemoteObserverDeletedCallback (
					RemoteReferencedObject_ptr observable,
					const std::vector<RemoteObjectBase_var>& list)
			: RemoteObserverCallback (observable, list)
		{
#ifdef OMNIORB
			start_undetached ( );
#else	// OMNIORB
#	error "Classe de base de threads non définie pour cet ORB"
#endif	// OMNIORB
		}	// RemoteObserverDeletedCallback

		/**
		 * Destructeur : RAS.
		 */
		virtual ~RemoteObserverDeletedCallback ( )
		{ }


		/**
		 * L'appel des observateurs.
		 */
#ifdef OMNIORB
		virtual void* run_undetached (void*);
#else	// OMNIORB
#	error "Classe de base de threads non définie pour cet ORB"
#endif	// OMNIORB


		private :

		/**
		 * Constructeurs de copie et opérateur = : interdits.
		 */
		RemoteObserverDeletedCallback (const RemoteObserverDeletedCallback&)
			: RemoteObserverCallback(0,std::vector<RemoteObjectBase_var> ( ))
		{
			assert(0&&"RemoteObserverDeletedCallback copy constructor is not allowed");
		}
		RemoteObserverDeletedCallback& operator = (
									const RemoteObserverDeletedCallback&)
		{
			assert(0&&"RemoteObserverDeletedCallback operator = is not allowed");
			return *this;
		}
	};	// class RemoteObserverDeletedCallback

	/**
	 * Appelle remoteObservableModified (this) de tous les objets qu'il
	 * référence.
	 * @param		Nature de la modification
	 */
	virtual void notifyRemoteObserversForModification (CORBA::ULong kind);

	/**
	 * Appelle remoteObservableDeleted (this) de tous les objets qu'il
	 * référence.
	 */
	virtual void notifyRemoteObserversForDestruction ( );


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	RemoteReferencedObjectImpl (const RemoteReferencedObjectImpl&);
	RemoteReferencedObjectImpl& operator = (const RemoteReferencedObjectImpl&);

	/** Les objets référencés appelés lors des modifications (y compris en cas
	 * de destruction. */
	std::vector<RemoteObjectBase_var>		_observers;
};	// class RemoteReferencedObjectImpl


/**
 * Implémentation d'une classe observable Corba dotée d'un nom.
 */
class RemoteReferencedNamedObjectImpl :
					public POA_CorbaFacilities::RemoteReferencedNamedObject,
					public RemoteReferencedObjectImpl
{
	public :

	/**
	 * Constructeur
	 * @param		Nom de l'objet
	 */
	RemoteReferencedNamedObjectImpl (const std::string& name = "unamed");

	/**
	 * Destructeur : RAS.
	 */
	virtual ~RemoteReferencedNamedObjectImpl ( );

	/**
	 * @return		Le nom de l'objet.
	 */
	virtual char* getRemoteName ( );

	/**
	 * @param		Le nouveau nom de l'objet
	 */
	virtual void setRemoteName (const char* name);


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	RemoteReferencedNamedObjectImpl (const RemoteReferencedNamedObjectImpl&);
	RemoteReferencedNamedObjectImpl& operator = (
							const RemoteReferencedNamedObjectImpl&);

	/**
	 * Le nom de l'instance. */
	std::string			_name;
};	// RemoteReferencedNamedObjectImpl

};	// namespace CorbaFacilities

#endif	// REMOTE_REFERENCED_OBJET_IMPL_H
