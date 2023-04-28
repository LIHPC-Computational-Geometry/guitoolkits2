#ifndef QT_ACTION_AUTO_LOCK_H
#define QT_ACTION_AUTO_LOCK_H

#ifndef QT_5
#include <QtGui/QAction>
#else	// QT_5
#include <QAction>
#endif	// QT_5


/** Classe permettant de verrouiller l'emission de signaux d'une action.
 * L'emission de signaux est automatiquement dévérouillée à la destruction de
 * l'instance.
 */
class QtActionAutoLock
{
	public :

	/**
	 * Constructeur : verrouille l'action transmise en argument.
	 */
	QtActionAutoLock (QAction* action);

	/**
	 * Destructeur : deverrouille l'action associée.
	 */
	virtual ~QtActionAutoLock ( );

	/**
	 * Verrouille l'emission de signaux si enabled vaut false, la déverrouille
	 * dans le cas contraire.
	 */
	virtual void setEnabled (bool enable);


	private :

	/** Constructeur de copie : interdit. */
	QtActionAutoLock (const QtActionAutoLock&)
	{ }

	/** Opérateur = : interdit. */
	QtActionAutoLock& operator = (const QtActionAutoLock&)
	{ return *this; }

	/** L'action associée. */
	QAction*				_action;
};	// class QtActionAutoLock


inline QtActionAutoLock::QtActionAutoLock (QAction* action)
	: _action (action)
{
	setEnabled (false);
}	// QtActionAutoLock::QtActionAutoLock


inline QtActionAutoLock::~QtActionAutoLock ( )
{
	setEnabled (true);
}	// QtActionAutoLock::~QtActionAutoLock


inline void QtActionAutoLock::setEnabled (bool enabled)
{
	if (0 != _action)
		_action->blockSignals (!enabled);
}	// QtActionAutoLock::setEnabled


#endif	// QT_ACTION_AUTO_LOCK_H
