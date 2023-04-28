#ifndef REFERENCED_OBJECT_H
#define REFERENCED_OBJECT_H

#include <TkUtil/util_config.h>

#include <map>
#include <vector>
#include <string>
#include <assert.h>


#ifndef SWIG
BEGIN_NAMESPACE_UTIL
#endif	// #ifndef SWIG


#ifdef MULTITHREADED_APPLICATION

class ReferencedMutex;

#define LOCK_INSTANCE  AutoReferencedMutex autoReferencedMutex (getMutex ( ));

#else

#define LOCK_INSTANCE

#endif	// MULTITHREADED_APPLICATION


class ReferencedObject;

/** L'évènement "non précisé" (valeur (unsigned long)0). */
#define UTIL_UNDEFINED_EVENT ((unsigned long)0)

/** L'évènement "objet indisponible" (valeur (unsigned long)1) : à ne plus
    utiliser. */
#define UTIL_UNAVAILABLE_EVENT ((unsigned long)1)

/** L'évènement "nom modifié" (valeur 2). */
#define UTIL_NAME_MODIFIED ((unsigned long)2)

/**
 * <P>Classe de base d'objets pouvant "observer" des objets fonctionnant avec un compteur de référence.</P>
 *
 * <P>Dans la version <B>multithread</B> de cette bibliothèque les instances de cette classe peuvent être protégées par des <B>mutex</B>. La détermination
 * de cette protection est effectuée à la construction des instances.
 * L'ensemble des méthodes de cette classe sont alors verrouillées, et la méthode <I>getMutex</I> permet à des classes dérivées de verrouiller
 * également leurs méthodes. Ce verrouillage peut utiliser par exemple la classe <I>AutoReferencedMutex</I> prévue à cet effet, ou directement la macro
 * <I>LOCK_INSTANCE</I> définie ci-dessus.
 * </P>
 * 
 * @warning		Il est vivement recommandé que le destructeur des classes dérivées invoquent <I>unregisterReferences</I> pour 
 * 				bénéficier pleinement du polymorphisme en cas de références bilatérales.
 * @see			ReferencedMutex
 * @see			AutoReferencedMutex
 * @see			LOCK_INSTANCE
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class ObjectBase
{
	public :

	/**
	 * Constructeur.
	 * @param		true si l'instance doit être protegeable par un mutex, dans un contexte multithread, false dans le cas contraire.
	 */
	ObjectBase (bool hasMutex = false);

	/**
	 * Destructeur. Appelle unregisterReferences.
	 */
	virtual ~ObjectBase ( );

	/**
	 * Appelé par un objet référencé lorsque celui-ci vient  d'être modifié.
	 * @param		Objet modifié
	 * @param		Nature de la ou des modification(s) (fonctionne avec l'opérateur |).
	 */
	virtual void observableModified (ReferencedObject* object, unsigned long event);

	/**
	 * Appelé par un objet référencé lorsque celui-ci est en cours de destruction.
	 */
	virtual void observableDeleted (ReferencedObject* object);

	/**
	 * Se désenregistre auprès de tous les objets qui le référencent.
	 */
	virtual void unregisterReferences ( );

	/**
	 * S'enregistre auprès de l'objet reçu en argument pour observer toutes ses modifications.
	 * @param		Objet observé
	 * @param		true si l'objet observé ne peut s'autodétruire tant que cette instance l'observe, sinon false.
	 */
	virtual void registerObservable (ReferencedObject* object, bool blockDestruction);

	/**
	 * Se désenregistre auprès de l'objet reçu en argument pour ne plus observer ses modifications.
	 * @param		Objet observé
	 * @param		true si l'objet observé peut s'autodétruire, sinon false.
	 */
	virtual void unregisterObservable (ReferencedObject* object, bool blockDestruction);

	/**
	 * @param		Observable évalué
	 * @param		aptitude à l'autodestruction évaluée.
	 * @return		true si l'observable est déjà enregistré, sinon false.
	 */
	virtual bool isObservableRegistered (ReferencedObject* object, bool blockDestruction) const;

	/**
	 * @return		Le nombre d'objets observés.
	 */
	virtual size_t getObservableCount ( ) const;

	/**
	 * @return		La liste des objets observés.
	 */
	virtual const IN_STD vector<ReferencedObject*> getObservables ( ) const;
	virtual IN_STD vector<ReferencedObject*> getObservables ( );

#ifdef MULTITHREADED_APPLICATION
	/**
	 * @return		Un pointeur sur le mutex verrouillant l'instance dans un contexte de mutithreading.
	 * @warning		Ce pointeur peut être nul. L'éventuelle protection par un mutex est déterminée par un paramètre transmis au constructeur.
	 */
	virtual ReferencedMutex* getMutex ( ) const
	{ return _mutex; }
#endif	// MULTITHREADED_APPLICATION

	/**
	 * <P>Vide la liste des observables en cas d'urgence, de perte de contrôle.
	 * Ce "ménage" sant effectué sans protection par mutex en environnement <I>multithread</I>. Afin de limiter autant que possible les risques de
	 * plantage <I>removeObserver</I> est appelé pour chaque objet observé avec <I>false</I> comme argument</P>
	 * <P>Méthode créée pour des conditions très particulières (annulation incontrôlée de <I>thread</I>), à éviter autant que possible.</P>
	 */
	virtual void emergencyCleanup ( );


	private :

	/**
	 * Constructeur de copie. Interdit.
	 */
	ObjectBase (const ObjectBase&);

	/**
	 * Opérateur =. Interdit.
	 */
	ObjectBase& operator = (const ObjectBase&);

	/** La liste des objets observés. */
	IN_STD multimap<ReferencedObject*, bool>		_observables;

#ifdef MULTITHREADED_APPLICATION
	mutable ReferencedMutex*						_mutex;
#endif	// MULTITHREADED_APPLICATION
};	// class ObjectBase


/**
 * <P>Classe de base d'objet fonctionnant avec un système de compteur de référence.</P>
 * <P>Les instances de cette classe se détruisent automatiquement lorsqu'elles ne sont plus référencées par d'autres objets qui
 * les "observent" et interdisent leur auto-destruction.</P>
 * <P>Les instances de cette classes sont également pourvues d'unmécanisme de callback visant à informer les objets qui les référencent lorsqu'elles sont modifiées.
 * </P>
 * @warning		Il est vivement recommandé que le destructeur des classes dérivées invoquent <I>notifyObserversForDestruction</I> pour 
 *				bénéficier pleinement du polymorphisme en cas de références bilatérales.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class ReferencedObject : public ObjectBase
{
	public :

	/**
	 * Constructeur par défaut.
	 * @param		true si l'instance doit être protegeable par un mutex, dans un contexte mutlithread, false dans le cas contraire.
	 */
	ReferencedObject (bool hasMutex = false);

	/**
	 * Destructeur. Invoque notifyObserversForDestruction.
	 */
	virtual ~ReferencedObject ( );

	/**
	 * Enregistre une référence sur l'objet reçu en argument. Cet observateur sera notifié de tout changement de cette instance.
	 * @param		Objet qui observe l'instance invoquée
	 * @param		true si cette instance ne peut s'autodétruire tant que l'objet reçu en argument l'observe, sinon false.
	 */
	virtual void addObserver (ObjectBase* object, bool blockDestruction);

	/**
	 * Désenregistre une référence sur l'objet reçu en argument.
	 * Se détruit s'il n'a plus aucune référence sur d'autres objets bloquant sa destruction.
	 * @param		Objet qui observait l'instance invoquée
	 * @param		true si cette instance ne peut s'autodétruire tant que l'objet reçu en argument l'observe, sinon false.
	 */
	virtual void removeObserver (ObjectBase* object, bool blockDestruction);

	/**
	 * @return		Le nombre d'objets qui observent l'instance.
	 */
	virtual size_t getObserverCount ( ) const;

	/**
	 * @return		La liste des objets qui observe l'instance.
	 */
	virtual IN_STD vector<ObjectBase*> getObservers ( ) const;

	/**
	 * Appellé en cas d'urgence avec situation peu contrôlée.
	 * <P>Appelle ObjectBase::emergencyCleanup.</P>
	 * <P>Vide la liste des observateurs. Ce "ménage" est effectué sans protection par mutex en environnement
	 * <I>multithread</I>. Afin de limiter autant que possible les risques de plantage <I>removeObserver</I> est appelé pour chaque objet observé avec
	 * <I>false</I> comme argument</P>
	 * <P>Méthode créée pour des conditions très particulières (annulation incontrôlée de <I>thread</I>), à éviter autant que possible.</P>
	 */
	virtual void emergencyCleanup ( );


	protected :

	/**
	 * Appelle observableModified (this) de tous les objets qu'il  référence.
	 * @param		Nature de la ou des modification(s) (fonctionne avec l'opérateur |).
	 */
	virtual void notifyObserversForModification (unsigned long event);

	/**
	 * Appelle observableDeleted (this) de tous les objets qu'il référence.
	 */
	virtual void notifyObserversForDestruction ( );

	/**
	 * @return		true si l'objet peut s'auto-détruire, sinon false.
	 */
	virtual bool allowsDestruction ( ) const;


	private :

	/** 
	 * Constructeur de copie et opérateur = interdits.
	 */
	ReferencedObject (const ReferencedObject&);
	ReferencedObject& operator = (const ReferencedObject&);

	/** Les objets référencés appelés lors des modifications (y
	 * compris en cas de destruction). */
	IN_STD multimap<ObjectBase*, bool>		_observers;
};	// class ReferencedObject


/**
 * <P>Classe d'objet référencé doté d'un nom. Un <B>nom unique</B> peut également être associé à l'instance via la méthode <I>setUniqueName</I>. Ce
 * nom unique peut par exemple décrire le chemin complet d'accès à un composant à partir d'un conteneur de base. Le nom unique de l'instance est par défaut
 * son nom, mais la surcharge de la méthode <I>getUniqueName</I>  peut permettre son adaptation dynamique au cours du programme (changement de nom, de nom de
 * ses "parents" ou autres. Le nom de l'instance peut par exemple affiché dans une IHM, alors que son nom unique sert à récupérer l'instance dans un <B>script</B>.
 * </P>
 * @see			ReferencedObjectManager
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class ReferencedNamedObject : public ReferencedObject
{
	public :

	/**
	 * Constructeur par défaut.
	 * @param		Nom de l'objet.
	 * @param		true si l'instance doit être protegeable par un mutex, dans un contexte multithread, false dans le cas contraire.
	 */
	ReferencedNamedObject (const IN_STD string& name = "unamed", bool hasMutex = false);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~ReferencedNamedObject ( );

	/**
	 * @return		Le nom de l'objet.
	 */
	virtual const IN_STD string& getName ( ) const
	{ return _name; }

	/**
	 * Invoque notifyObserversForModification.
	 * @param		Nouveau nom de l'objet.
	 */
#ifndef SWIG
	virtual void setName (const IN_STD string& name);
#else	// SWIG
	virtual void setName (const char* name);
#endif	// #ifndef SWIG

	/**
	 * @return		Le nom unique de l'objet.
	 * @see			setUniqueName
	 */
	virtual IN_STD string getUniqueName ( ) const
	{ return _uniqueName; }

	/**
	 * Invoque notifyObserversForModification.
	 * @param		Nouveau nom unique de l'objet.
	 */
#ifndef SWIG
	virtual void setUniqueName (const IN_STD string& name);
#else	// SWIG
	virtual void setUniqueName (const char* name);
#endif	// #ifndef SWIG


	private :

	/** 
	 * Constructeur de copie et opérateur = interdits.
	 */
	ReferencedNamedObject (const ReferencedNamedObject&);
	ReferencedNamedObject& operator = (const ReferencedNamedObject&);

	/** Le nom de l'objet. */
	IN_STD string		_name;

	/** Le nom unique de l'objet. */
	IN_STD string		_uniqueName;
};	// ReferencedNamedObject


/**
 * <P>Classe à instance unique permettant d'accéder via un nom unique à une instance de la classe ReferencedObject qui s'est enregistrée auprès de ce service.
 * </P>
 * <P>Les services de cette classe présentent notamment un intérêt lors d'exécution de <B>scripts</B> dans un logiciel. Ils permettent au script
 * d'accéder grâce à la fonction getObject aux instances chargées en mémoire.
 * </P>
 * <P>Cette classe peut être dérivée en vue d'y ajouter des accesseurs  spécialisés au niveau du type retourné, ou d'informer d'autres instances
 * du chargement via un script d'objets.</P>.
 * <P>A noter que cette classe ne gère ni l'instanciation, ni la destruction des instances de la classe ReferencedNamedObject.
 * </P>
 */
class ReferencedObjectManager : protected ReferencedObject
{
	public :

	/**
	 * Constructeur. RAS.
	 */
	ReferencedObjectManager ( );

	/**
	 * Destructeur : RAS.
	 */
	virtual ~ReferencedObjectManager ( );

	/**
	 * @return		Une référence sur l'instance unique de cette classe.
	 * @exception	Une exception est levée si l'API n'est pas fonctionnelle
	 */
	static ReferencedObjectManager& getManagerInstance ( );

	/**
	 * @param		Objet à enregistrer auprès du gestionnaire. Le déréférencement se fait via <I>unregisterObject</I> ou automatiquement à sa destruction.
	 * @exception	Une exception est levée si un objet différent mais de même nom unique est déjà enregistré auprès de ce service.
	 * @see			unregisterObject
	 */
	virtual void registerObject (ReferencedNamedObject* object);

	/**
	 * @param		Objet à déréférencer auprès du gestionnaire.
	 * @see			unregister
	 */
	virtual void unregisterObject (ReferencedNamedObject* object);

	/**
	 * @return		Un pointeur sur l'instance dont le nom unique est transmis en argument.
	 * @exception	Une exception est levée si l'instance demandée n'est pas référencée auprès du gestionnaire.
	 */
#ifndef SWIG
	virtual ReferencedNamedObject* getInstance (const IN_STD string& name);
#else	// SWIG
	virtual ReferencedNamedObject* getInstance (const char* name);
#endif	// #ifndef SWIG


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	ReferencedObjectManager (const ReferencedObjectManager&);
	ReferencedObjectManager& operator = (const ReferencedObjectManager&);

	/** L'instance unique du gestionnaire. */
	static ReferencedObjectManager*			_instance;
};	// class ReferencedObjectManager : public ObjectBase


/**
* @return		Une référence sur l'instance unique de la classe <I>ReferencedObjectManager</I..
* @exception	Une exception est levée si l'API n'est pas fonctionnelle
*/
extern ReferencedObjectManager& getObjectManagerInstance ( );


#ifndef SWIG
END_NAMESPACE_UTIL
#endif	// #ifndef SWIG

#endif	// REFERENCED_OBJECT_H
