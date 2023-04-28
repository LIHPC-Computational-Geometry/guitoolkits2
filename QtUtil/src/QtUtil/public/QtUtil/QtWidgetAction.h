/**
 * @file		QtWidgetAction.h
 * @author		Charles PIGNEROL
 * @date		13/01/2015
 */


#ifndef QT_WIDGET_ACTION_H
#define QT_WIDGET_ACTION_H

#ifndef QT_5
#include <QtGui/QWidgetAction>
#else	// QT_5
#include <QWidgetAction>
#endif	// QT_5



/**
 * <P>
 * Classe de <I>QAction</I> créant un widget associé de type <I>QToolButton</I>.
 * Classe destinée à fonctionner avec la classe <I>QtMultiLineToolbar</I>.
 * </P>
 */
class QtWidgetAction : public QWidgetAction
{
	public :

	/**
	 * Constructeur
	 * @param		parent
	 */
	QtWidgetAction (QWidget* parent);

	/**
	 * Constructeur
	 * @param		icône représentative
	 * @param		texte descriptif
	 * @param		parent
	 */
	QtWidgetAction (const QIcon& icon, const QString& text, QObject* parent);

	/**
	 * Destructeur
	 */
	virtual ~QtWidgetAction ( );


	protected :

	/**
	 * Créé un widget pour être représenté.
	 * @param		Parent du widget créé
	 */
	virtual QWidget* createWidget (QWidget* parent);


	private :

	/** Constructeur de copie. Interdit. */
	QtWidgetAction (const QtWidgetAction&);

	/** Opérateur de copie. Interdit. */
	QtWidgetAction& operator = (const QtWidgetAction&);
};	// class QtWidgetAction


#endif	// QT_WIDGET_ACTION_H

