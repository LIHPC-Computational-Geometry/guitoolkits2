#ifndef QT_OBJECT_SIGNAL_BLOCKER_H
#define QT_OBJECT_SIGNAL_BLOCKER_H

#include <QObject>


/** Classe permettant d'empêcher un objet d'envoyer des signaux. L'objet
 * est automatiquement dévérouillé à la destruction de l'instance.
 */
class QtObjectSignalBlocker
{
	public :

	/**
	 * Constructeur : empêche l'objet transmis en argument d'envoyer des signaux.
	 */
	QtObjectSignalBlocker (QObject* object);

	/**
	 * Destructeur : deverrouille le object associé.
	 */
	virtual ~QtObjectSignalBlocker ( );

	/**
	 * Empêche l'objet d'envoyer des signaux si enabled vaut false,
	 * l'autorise dans le cas contraire.
	 */
	virtual void setEnabled (bool enable);


	private :

	/** Constructeur de copie : interdit. */
	QtObjectSignalBlocker (const QtObjectSignalBlocker&)
	{ }

	/** Opérateur = : interdit. */
	QtObjectSignalBlocker& operator = (const QtObjectSignalBlocker&)
	{ return *this; }

	/** L'objet associé. */
	QObject*				_object;
};	// class QtObjectSignalBlocker


inline QtObjectSignalBlocker::QtObjectSignalBlocker (QObject* object)
	: _object (object)
{
	setEnabled (false);
}	// QtObjectSignalBlocker::QtObjectSignalBlocker


inline QtObjectSignalBlocker::~QtObjectSignalBlocker ( )
{
	setEnabled (true);
}	// QtObjectSignalBlocker::~QtObjectSignalBlocker


inline void QtObjectSignalBlocker::setEnabled (bool enabled)
{
	if (0 != _object)
		_object->blockSignals (!enabled);
}	// QtObjectSignalBlocker::setEnabled


#endif	// QT_OBJECT_SIGNAL_BLOCKER_H
