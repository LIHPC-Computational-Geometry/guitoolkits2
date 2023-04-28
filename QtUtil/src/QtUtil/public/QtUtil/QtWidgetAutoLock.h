#ifndef QT_WIDGET_AUTO_LOCK_H
#define QT_WIDGET_AUTO_LOCK_H

#ifndef QT_5
#include <QtGui/QWidget>
#else	// QT_5
#include <QWidget>
#endif	// QT_5


/** Classe permettant de verrouiller un widget, par exemple un menu. Le widget
 * est automatiquement dévérouillé à la destruction de l'instance.
 */
class QtWidgetAutoLock
{
	public :

	/**
	 * Constructeur : verrouille le widget transmis en argument.
	 */
	QtWidgetAutoLock (QWidget* widget);

	/**
	 * Destructeur : deverrouille le widget associé.
	 */
	virtual ~QtWidgetAutoLock ( );

	/**
	 * Verrouille le widget si enabled vaut false, le déverrouille dans le cas
	 * contraire.
	 */
	virtual void setEnabled (bool enable);


	private :

	/** Constructeur de copie : interdit. */
	QtWidgetAutoLock (const QtWidgetAutoLock&)
	{ }

	/** Opérateur = : interdit. */
	QtWidgetAutoLock& operator = (const QtWidgetAutoLock&)
	{ return *this; }

	/** Le widget associé. */
	QWidget*				_widget;
};	// class QtWidgetAutoLock


inline QtWidgetAutoLock::QtWidgetAutoLock (QWidget* widget)
	: _widget (widget)
{
	setEnabled (false);
}	// QtWidgetAutoLock::QtWidgetAutoLock


inline QtWidgetAutoLock::~QtWidgetAutoLock ( )
{
	setEnabled (true);
}	// QtWidgetAutoLock::~QtWidgetAutoLock


inline void QtWidgetAutoLock::setEnabled (bool enabled)
{
	if (0 != _widget)
		_widget->setEnabled (enabled);
}	// QtWidgetAutoLock::setEnabled


#endif	// QT_WIDGET_AUTO_LOCK_H
