#ifndef QT_TEXT_FIELD_H
#define QT_TEXT_FIELD_H

#include <TkUtil/util_config.h>

#ifndef QT_5
#include <QtGui/QLineEdit>
#else	// QT_5
#include <QLineEdit>
#endif	// QT_5



/**
 * Editeur de texte Qt offrant davantage de possibilités que la classe
 * QLineEdit.
 */
class QtTextField : public QLineEdit 
{
	public :

	/** Constructeur
	 * @param		widget parent.
	 * @param		nom du widget.
	 */
	QtTextField (QWidget* parent, const char* name = 0);

	/** Constructeur
	 * @param		texte à afficher.
	 * @param		widget parent.
	 * @param		nom du widget.
	 */
	QtTextField(const QString& contents, QWidget* parent, const char* name = 0);

	/** Destructeur.
	 */
	virtual ~QtTextField ( );

	/**
	 * @return		Le texte du champs de texte.
	 */
	virtual IN_STD string getText ( ) const;
	virtual QString getQText ( ) const;

	/**
	 * @param		Le nouveau texte du champs de texte.
	 */
	virtual void setText (const IN_STD string& text);
	virtual void setText (const QString& text);

	/** Ajuste la taille minimale.
	 * @param		nombre de caractères visibles.
	 * @see			sizeHint
	 */
	virtual void setVisibleColumns (int count);

	/**
	 * @return		Les largeurs/hauteurs correspondant au nombre de caractères
	 *			visibles demandés. En l'absence de demande spécifique, via
	 *			<I>setVisibleColumns</I>, retourne <I>QLineEdit::sizeHint ( )</I>
	 *			(ce qui correspond à une largeur d'environ 17 caractères).
	 *			Si le champ de texte a du texte prend la taille des maxLength ( )
	 *			premiers caractères.
	 * @see			isNumeric
	 */
	 virtual QSize sizeHint ( ) const;

	/**
	 * @return		<I>true</I> si le champ contient des valeurs numériques,
	 *				<I>false</I> dans le cas contraire. Retourne <I>false</I>
	 *				par défaut, méthode à surcharger si nécessaire.
	 * Utile pour <I>sizeHint</I>.
	 * @see			sizeHint
	 */
	virtual bool isNumeric ( ) const;


	private :

	/** Constructeur de copie. Interdit. */
	QtTextField (const QtTextField&);

	/** Opérateur de copie. Interdit. */
	QtTextField& operator = (const QtTextField&);
};	// class QtTextField



#endif	// QT_TEXT_FIELD_H
