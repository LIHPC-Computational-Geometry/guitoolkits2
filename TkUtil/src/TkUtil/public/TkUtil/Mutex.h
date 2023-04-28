#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

#include <TkUtil/util_config.h>


BEGIN_NAMESPACE_UTIL


/**
 * <P>Classe de verrou posix de base de type PTHREAD_MUTEX_RECURSIVE
 * (défaut) ou PTHREAD_MUTEX_ERRORCHECK.
 * </P>
 * <P>Un mutex récursif gère un compteur qui autorise de multiple opérations
 * successives de verrouillage au sein d'un même thread. Pour libérer le
 * verrou il convient de le déverrouiller autant de fois qu'il a été verrouillé.
 * </P>
 *
 * @see		man pthread.h
 * @see		ReferencedMutex
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Mutex
{
	public :

	/**
	 * Constructeur. Créé le mutex sans le verrouiller. _lockCount est égal 
	 * à 0.
	 * @param	<I>true</I> si le mutex est récursif (PTHREAD_MUTEX_RECURSIVE),
	 *			<I>false</I> dans le cas contraire.
	 */
	Mutex (bool recursive = true);

	/**
	 * Destructeur. Appelle release.
	 * @see		release
	 */
	virtual ~Mutex ( );

	/**
	 * Aquiert un verrou. Incrémente _lockCount.
	 * @see		tryLock
	 * @see		unlock
	 */
	virtual void lock ( );

	/**
	 * Essaie d'aquérir un verrou. Incrémente _lockCount en cas de succès.
	 * @return	<I>true</I> en cas d'aquisition du verrou, <I>false</I> en
	 *			cas d'échec.
	 * @see		lock
	 * @see		unlock
	 */
	virtual bool tryLock ( );

	/**
	 * Libère le verrou. Décrémente _lockCount.
	 * @see		lock
	 * @see		tryLock
	 * @see		safeUnlock
	 */
	virtual void unlock ( );

	/**
	 * @return		Le mutex représenté.
	 */
	pthread_mutex_t mutex ( ) const;

	/**
	 * @return		Le nombre d'options de verrouillage en cours.
	 */
	virtual size_t getLockCount ( ) const
	{ return _lockCount; }

	/**
	 * @return		-1 si le mutex n'est pas verrouillé, ou l'identifiant du
	 *				thread qui l'a verrouillé.
	 */
	virtual pthread_t getOwner ( ) const
	{ return _owner; }

	/**
	 * @return		true si le mutex est libéré, sinon false.
	 * @see			release
	 */
	virtual bool isReleased ( ) const
	{ return _released; }

	/**
	 * Libère le mutex. Appelle unlock tant que le compteur n'est pas nul puis
	 * détruit le mutex.
	 * @warning		Cette instance n'est plus utilisable après cet appel.
	 * @see			isReleased
	 */
	virtual void release ( );


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	Mutex (const Mutex&);
	Mutex& operator = (const Mutex&);

	/** Le verrou. */
	pthread_mutex_t		_mutex;

	/** Le nombre d'appels à <I>lock</I> en cours. */
	size_t				_lockCount;

	/** Le thread qui détient le verrou. */
	pthread_t			_owner;

	/** Le mutex est-il libéré ? */
	bool				_released;
};	// class Mutex


/**
 * <P>Classe permettant de gérer une séquence <I>lock</I>/<I>unlock</I> de mutex
 * dans tout contexte, y compris en cas de levée d'exception.
 * </P>
 * <P>Exemple d'utilisation :<BR>
 * <CODE>
 * static Mutex*	criticalMutex	= new Mutex ( );<BR>
 * ...<BR>
 * void function ( )<BR>
 * {<BR>
 *		// Le verrou est pris en entrant dans <I>function</I>.<BR>
 * 		<B>AutoMutex	autoMutex (*criticalMutex);</B><BR>
 *		...<BR>
 *		// code de <I>function</I>.<BR>
 *		...<BR>
 *		// Le verrou est libéré en sortant de <I>function</I>, même si une<BR>
 *		// exception est levée.<BR>
 * }<BR>
 * </CODE>
 * </P>
 * 
 * @warning		Ne pas utiliser cette classe dans un contexte de destruction.
 *				Si un AutoMutex verrouille une instance qui est détruite il
 *				appelera malgré tout <I>mutex->unlock ( )</I> lors de sa
 *				propre destruction. La classe <I>AutoReferencedMutex</I>,
 *				qui utilise un système de comptage de références avec
 *				callbacks, est prévue pour ce type de situations.
 * @see			AutoReferencedMutex
 */
class AutoMutex
{
	public :

	/**
	 * Verrouille le mutex tranmis en argument.
	 */
	AutoMutex (Mutex* mutex)
		: _mutex (mutex)
	{
		if (0 != _mutex)
			mutex->lock ( );
	}

	/**
	 * Déverrouille le mutex.
	 */
	~AutoMutex ( )
	{
		if (0 != _mutex)
			_mutex->unlock ( );
	}


	private :

	/**
	 * Opérateur = et constructeur de copie : interdits.
	 */
	AutoMutex (const AutoMutex&);
	AutoMutex& operator = (const AutoMutex&);

	/** Le mutex géré. */
	Mutex*			_mutex;
};	// class AutoMutex


END_NAMESPACE_UTIL


#endif	// MUTEX_H
