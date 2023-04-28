#ifndef REFERENCED_MUTEX_H
#define REFERENCED_MUTEX_H

#include <TkUtil/Mutex.h>
#include <TkUtil/ReferencedObject.h>


BEGIN_NAMESPACE_UTIL


/**
 * <P>Classe de mutex fonctionnant avec un système de comptage de références.
 * Ce type de mutex prévient ses observateurs de sa destruction afin que ceux-ci
 * n'invoquent plus ses méthodes. La classe <I>AutoReferencedMutex</I> permet
 * de protéger une section pouvant comprendre l'éventuelle destruction du mutex.
 * </P>
 * <P>Le système de comptage de référence est délégué à la classe
 * <I>ReferencedObject</I> dont hérite cette classe.
 * </P>
 * @see		Mutex
 * @see		ReferencedObject
 * @see		AutoReferencedMutex
 */
class ReferencedMutex : public Mutex, public ReferencedObject
{
	public :

	/**
	 * Constructeur. Appelle <I>ObjectBase (false)</I> (le mutex n'est pas
	 * protégé lui même par un autre mutex).
	 */
	ReferencedMutex ( );

	/**
	 * Destructeur. Notifie ses éventuels observateurs de sa destruction.
	 */
	virtual ~ReferencedMutex ( );

	/**
	 * @return		false afin d'interdire son auto-destruction.
	 */
	virtual bool allowsDestruction ( ) const
	{ return false; }


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	ReferencedMutex (const ReferencedMutex&);
	ReferencedMutex& operator = (const ReferencedMutex&);
};  // class ReferencedMutex


/**
 * <P>Classe permettant de gérer une séquence <I>lock</I>/<I>unlock</I> de mutex
 * dans tout contexte, y compris en cas de levée d'exception ou en cas de 
 * destruction possible du mutex durant l'exécution du code protégé. En cas
 * de destruction du mutex, l'instance de cette classe en est informée et ne
 * tente pas de déverrouiller le mutex lors de sa destruction.
 *</P>
 * @see		ReferencedMutex
 */
class AutoReferencedMutex : public ObjectBase
{
	public :

	/**
	 * Constructeur. S'enregistre comme observateur du mutex reçu en argument,
	 * puis verrouille celui-ci.
	 */
	AutoReferencedMutex (ReferencedMutex* mutex);

	/**
	 * Destructeur. Déverrouille le mutex géré (si celui-ci n'a pas été détruit.
	 */
	virtual ~AutoReferencedMutex ( );

	/**
	 * Déréférence le mutex géré si celui-ci est tranmis en argument.
	 */
	virtual void observableDeleted (ReferencedObject* object);


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	AutoReferencedMutex (const AutoReferencedMutex&);
	AutoReferencedMutex& operator = (const AutoReferencedMutex&);

	/** Le mutex géré. */
	ReferencedMutex*		_mutex;
};	// class AutoReferencedMutex


END_NAMESPACE_UTIL


#endif	// REFERENCED_MUTEX_H
