#ifndef QT_RICH_TEXT_COMBO_BOX_H
#define QT_RICH_TEXT_COMBO_BOX_H

#ifndef QT_5
#include <QtGui/QComboBox>
#include <QtGui/QItemDelegate>
#else	// QT_5
#include <QComboBox>
#include <QItemDelegate>
#endif	// QT_5


/**
 * <P>Classe de boite à défilement permettant d'afficher du texte enrichi et
 * donc d'utiliser par exemple différentes polices de caractères, même au sein
 * d'un même item.
 * </P>
 *
 * <P>Exemple d'utilisation :<BR>
 * <CODE>
 * QtRichTextComboBox*	comboBox	= new new QtRichTextComboBox (this);<BR>
 * // Fonte à utiliser en l'absence de fonte spécifique :<BR>
 * const string			fontFamily	=
 *					comboBox->font ( ).defaultFamily ( ).toStdString ( );<BR>
 * // Texte avec police type "symbôles" :<BR>
 * QString	richText	= QString ("min(") + QtStringHelper::richTextRoMin( ) +
 *		"1, " + QtStringHelper::richTextRoMin( ) + "2)";<BR>
 * comboBox->addItem (richText);<BR>
 * richText	= QtStringHelper::textToRichText ("texte normal", fontFamily);<BR>
 * comboBox->addItem (richText);<BR>
 * ...<BR>
 * </CODE>
 * </P>
 * @warning		<B>Tous les services de la classe <I>QComboBox</I> ne sont pas
 *				réimplémentés : gestion d'icônes, alignement du texte, ...</B>
 */
class QtRichTextComboBox : public QComboBox
{
	public :

	/**
	 * Constructeur. RAS.
	 */
	QtRichTextComboBox (QWidget* parent = 0);

	/**
	 * @return		Les tailles souhatitées pour ce widget.
	 */
	virtual QSize sizeHint ( ) const;
	virtual QSize minimumSizeHint ( ) const;

	/**
	 * Réaffiche le contenu du widget.
	 */
	virtual void paintEvent (QPaintEvent*);
};	// class QtRichTextComboBox


/**
 * Classe gérant l'affichage des items d'une boite à défilement type
 * QtRichTextComboBox.
 *
 * @warning		<B>Gère le texte enrichi, mais présente les mêmes limitations
 *				que la classe <I>QtRichTextComboBox</I>.</B>
 */
class QtComboBoxItemDelegate : public QItemDelegate
{
	public :

	/**
	 * Constructeur. RAS.
	 */
	QtComboBoxItemDelegate (QObject* parent);

	/**
	 * @return	La taille souhaitée pour la zone correspondant aux arguments
	 *			tranmis.
	 */
	virtual QSize sizeHint (
		const QStyleOptionViewItem & option, const QModelIndex& index ) const;

	/**
	 * Affiche a zone correspondat aux arguments tranmis.
	 */
	virtual void paint (QPainter* painter, const QStyleOptionViewItem& option,
	                    const QModelIndex& index ) const;
};	// class QtComboBoxItemDelegate

#endif	// QT_RICH_TEXT_COMBO_BOX_H
