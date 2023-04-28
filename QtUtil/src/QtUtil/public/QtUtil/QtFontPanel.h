// NO_QT3_MOC  Tag utilisé par le filtre MocFilter.py

#ifndef QT_FONT_PANEL_H
#define QT_FONT_PANEL_H

#include "QtUtil/QtIntTextField.h"

#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QCheckBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QApplication>
#include <QCheckBox>
#include <QFontComboBox>
#include <QPushButton>
#endif	// QT_5


/**
 * <P>Classe de panneau permettant de paramétrer une police de caractères Qt
 * caractérisé par une famille, une taille et une couleur.
 * </P>
 * @warning		Cette classe ne traque pas les exceptions et il convient aux
 *				utilisateurs de le faire.
 */
class QtFontPanel : public QWidget
{
	Q_OBJECT

	public :

	enum PANEL_ORIENTATION {HORIZONTAL_PANEL, VERTICAL_PANEL};

	/**
	 * Constructeur par défaut.
	 * @param		Widget parent.
	 * @param		Fonte par défaut.
	 * @param		Couleur par défaut.
	 * @param		Orientation du panneau.
	 */
	QtFontPanel (QWidget* parent, const QFont& font = QApplication::font ( ),
	             const QColor& color = Qt::black,
	             PANEL_ORIENTATION orientation = HORIZONTAL_PANEL);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtFontPanel ( );

	/**
	 * @return		<I>true</I> si le choix de la couleur est possible (valeur
	 *				par défaut, sinon <I>false</I>.
	 * @see			enableColor
	 */
	virtual bool isColorEnabled ( ) const;

	/**
	 * @param		<I>true</I> si le choix de la couleur est possible (valeur
	 *				par défaut, sinon <I>false</I>.
	 * @see			isColorEnabled
	 */
	virtual void enableColor (bool enable);

	/**
	 * @return		La couleur de la fonte.
	 * @see			setFont
	 */
	virtual QFont getFont ( ) const;

	/**
	 * @param		La nouvelle couleur de la fonte.
	 * @see			getFont
	 */
	virtual void setFont (const QFont& font);

	/**
	 * @return		La taille de la fonte.
	 * @see			setSize
	 */
	virtual unsigned int getSize ( ) const;

	/**
	 * @param		La nouvelle taille de la fonte.
	 * @see			getSize
	 */
	virtual void setSize (unsigned int size);

	/**
	 * @return		<I>true</I> si la police est en caractères gras,
	 *				<I>false</I> dans le cas contraire.
	 * @see			setBold
	 */
	virtual bool isBold ( ) const;

	/**
	 * @param		Nouveau caractère gras de la police de caractères.
	 * @see			isBold
	 */
	virtual void setBold (bool bold);

	/**
	 * @return		<I>true</I> si la police est en caractères italiques,
	 *				<I>false</I> dans le cas contraire.
	 * @see			setItalic
	 */
	virtual bool isItalic ( ) const;

	/**
	 * @param		Nouveau caractère italiques de la police de caractères.
	 * @see			isItalic
	 */
	virtual void setItalic (bool italic);


	/**
	 * @return		<I>true</I> si la police est en caractères soulignés,
	 *				<I>false</I> dans le cas contraire.
	 * @see			setUnderline
	 */
	virtual bool isUnderline ( ) const;

	/**
	 * @param		Nouveau caractère soulignés de la police de caractères.
	 * @see			isUnderline
	 */
	virtual void setUnderline (bool undeline);

	/**
	 * @return		La couleur de la fonte.
	 * @see			setColor
	 */
	virtual QColor getColor ( ) const;

	/**
	 * @param		La nouvelle couleur de la fonte.
	 * @see			getColor
	 */
	virtual void setColor (const QColor& color);


	protected slots :

	/**
	 * Appellé lorsuqe l'utilisateur clique sur le bouton "Couleur ...".
	 * Affiche une boite de dialogue d'édition de la couleur et suit les
	 * instructions de l'utilisateur.
	 */
	virtual void fontColorCallback ( );


	protected :

	/**
	 * Créé le panneau.
	 * @param		Fonte par défaut.
	 * @param		Couleur par défaut.
	 * @param		Orientation du panneau.
	 */
	virtual void createGui (const QFont& font, PANEL_ORIENTATION orientation);


	private :

	/** Constructeurs de copie interdits. */
	QtFontPanel (const QtFontPanel&);
	QtFontPanel& operator = (const QtFontPanel&);

	/** Le couleur de la fonte. */
	QColor					_color;

	/** La famille de la fonte. */
	QFontComboBox*			_familyComboBox;

	/** La taille de la fonte. */
	QtIntTextField*			_sizeTextField;

	/** Les cases à cocher "Gras", "Italique", "Souligné". */
	QCheckBox				*_boldCheckBox, *_italicCheckBox,
							*_underlineCheckBox;

	/** Le bouton "Couleur ...". */
	QPushButton*			_colorButton;
};	// class QtFontPanel

#endif	// QT_FONT_PANEL_H

