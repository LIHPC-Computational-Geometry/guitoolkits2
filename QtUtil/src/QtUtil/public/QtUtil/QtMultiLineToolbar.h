/**
 * @file		QtMultiLineToolbar.h
 * @author		Charles PIGNEROL
 * @date		13/01/2015
 */


#ifndef QT_MULTILINE_TOOLBAR_H
#define QT_MULTILINE_TOOLBAR_H

#ifndef QT_5
#include <QtGui/QWidgetAction>
#else	// QT_5
#include <QWidgetAction>
#endif	// QT_5



/**
 * <P>
 * Classe de <I>QToolBar</I> gérant le placement des icônes à l'aide d'une
 * instance de <I>QtFlowLayout</I>.
 * </P>
 */
class QtMultiLineToolbar : public QWidget
{
	public :

	/**
	 * Constructeur
	 * @param		parent.
	 */
	QtMultiLineToolbar (QWidget* parent);

	/** Destructeur.
	 */
	virtual ~QtMultiLineToolbar ( );

	/**
	 * Ajoute l'action transmise en argument.
	 */
	virtual void addAction (QWidgetAction& action);

	/**
	 * Ajoute un séparateur.
	 */
	virtual void addSeparator ( );

	/**
	 * Taille des icônes dans la barre.
	 */
	const QSize& iconSize ( ) const;
	void setIconSize (const QSize& iconSize);


	private :

	/** Constructeur de copie. Interdit. */
	QtMultiLineToolbar (const QtMultiLineToolbar&);

	/** Opérateur de copie. Interdit. */
	QtMultiLineToolbar& operator = (const QtMultiLineToolbar&);

	/** La taille des icônes dans la barre. */
	QSize					_iconSize;
};	// class QtMultiLineToolbar


#endif	// QT_MULTILINE_TOOLBAR_H

