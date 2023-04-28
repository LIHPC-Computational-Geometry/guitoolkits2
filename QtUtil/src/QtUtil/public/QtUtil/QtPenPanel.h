// NO_QT3_MOC  Tag utilisé par le filtre MocFilter.py

#ifndef QT_PEN_PANEL_H
#define QT_PEN_PANEL_H

#include <QtUtil/QtIntTextField.h>

#ifndef QT_5
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#endif	// QT_5


/**
 * <P>Classe de panneau permettant de paramétrer un stylo Qt caractérisé par
 * une couleur et une épaisseur de trait.
 * </P>
 * <P>Les paramètres peuvent prendre des <I>valeurs indéfinies</I> si cela est
 * spécifié au constructeur. Cela peut être utile lorsque le panneau sert à
 * paramétrer plusieurs instances. Par exemple, si le panneau permet à un
 * utilisateur de paramétrer le style de traits d'un ensemble de courbes. Ce
 * dispositif permet d'affecter la même épaisseur à l'ensemble des courbes et
 * de preciser à l'API appelante qu'il ne faut pas toucher à la couleur
 * (<I>getColor</I> retourne une couleur invalide).
 * <P>
 * @warning		Cette classe ne traque pas les exceptions et il convient aux
 *				utilisateurs de le faire.
 */
class QtPenPanel : public QWidget
{
	Q_OBJECT


	public :

	/**
	 * Constructeur par défaut. Si <I>indeterminate</I> vaut <I>true</I> il
	 * convient si nécessaire de transmettre les valeurs indéterminées aux
	 * arguments transmis.
	 * @param		Widget parent.
	 * @param		Orientation du panneau
	 * @param		<I>true</I> si les valeurs peuvent être indéfinies,
	 *				<I>false</I> dans le cas contraire.
	 * @param		Couleur par défaut (couleur invalide si indéterminée).
	 * @param		Epaisseur par défaut (<I>-1</I> si indéterminée).
	 */
	QtPenPanel (QWidget* parent, Qt::Orientations orientation,
	            bool indeterminate, const QColor color, uint width);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtPenPanel ( );

	/**
	 * @return		<I>true</I> si les paramètres peuvent être indéterminés,
	 *				<I>false</I> dans le cas contraire.
	 */
	virtual bool allowsUndeterminate ( ) const
	{ return _allowsUndeterminate; }

	/**
	 * @return		La couleur du stylo. Retourne une couleur invalide si
	 *				indéterminé, à condition qu'une couleur invalide soit
	 *				tranmise au constructeur ou par <I>setColor</I>.
	 * @see			setColor
	 */
	virtual QColor getColor ( ) const;

	/**
	 * @param		La nouvelle couleur du stylo.
	 * @see			getColor
	 */
	virtual void setColor (const QColor& color);

	/**
	 * @return		L'épaisseur du stylo, ou <I>-1</I si indéterminé.
	 * @see			setWidth
	 */
	virtual uint getWidth ( ) const;

	/**
	 * @param		La nouvelle épaisseur du stylo.
	 * @see			getWidth
	 * @warning		Attention, susceptible de lever une exception
	 */
	virtual void setWidth (uint width);

	/**
	 * @return		Le style du stylo, ou <I>(Qt::PenStyle)-1</I> si
	 *				indéterminé.
	 * @see			setStyle
	 */
	virtual Qt::PenStyle getStyle ( ) const;

	/**
	 * @param		Le nouveau style du stylo, ou <I>(Qt::PenStyle)-1</I> si
	 *				indéterminé.
	 * @see			getStyle
	 * @warning		Attention, susceptible de lever une exception
	 */
	virtual void setStyle (Qt::PenStyle style);

	/**
	 * @return		Le stylo représenté par ce panneau.
	 * @see			setPen
	 */
	virtual QPen getPen ( ) const;

	/**
	 * @param		Le nouveau stylo représenté par ce panneau.
	 * @see			getPen
	 */
	virtual void setPen (const QPen& pen);


	protected slots :

	/**
	 * Appellé lorsque l'utilisateur clique sur le bouton "Couleur ...".
	 * Affiche une boite de dialogue d'édition de la couleur et suit les
	 * instructions de l'utilisateur.
	 */
	virtual void editColorCallback ( );

	/**
	 * Appelé lorsque l'utilisateur (dé)coche la case "couleur indéterminée.
	 * Met à jour le caractère actif du bouton "Couleur".
	 */
	virtual void undeterminateColorCallback (int);


	private :

	/** Constructeurs de copie interdits. */
	QtPenPanel (const QtPenPanel&);
	QtPenPanel& operator = (const QtPenPanel&);

	/** <I>true</I> si les paramètres peuvent être indéterminés, <I>false</I>
	 * dans le cas contraire. */
	bool					_allowsUndeterminate;

	/** Le couleur du stylo. */
	QColor					_color;

	/** Le bouton "Couleur ...". */
	QPushButton*			_colorButton;

	/** La case à cocher "couleur indéterminée". */
	QCheckBox*				_undeterminateColorCheckbox;

	/** L'épaisseur du stylo. */
	QtIntTextField*			_widthTextField;

	/** Le style de trait. */
	QComboBox*				_styleComboBox;
};	// class QtPenPanel

#endif	// QT_PEN_PANEL_H
