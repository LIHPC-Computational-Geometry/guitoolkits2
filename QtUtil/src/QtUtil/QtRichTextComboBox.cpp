
#include "QtUtil/QtRichTextComboBox.h"
#include "QtUtil/QtConfiguration.h"
#include "TkUtil/util_config.h"

#include <QPainter>
#include <QPalette>
#include <QAbstractTextDocumentLayout>
#include <QTextDocument>
#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QStyle>
#include <QtGui/QStylePainter>
#else	// QT_5
#include <QApplication>
#include <QStyle>
#include <QStylePainter>
#endif	// QT_5

#include <assert.h>



/**
 * algos fortement inspirés des sources Qt 4.3.2 des classes :
 * - QLabel : gestion du "rich text"
 * - QItemDelegate : affichages des instances de cette classe
 */

QtRichTextComboBox::QtRichTextComboBox (QWidget* parent)
	: QComboBox (parent)
{
	setItemDelegate (new QtComboBoxItemDelegate (this));
}	// QtRichTextComboBox::QtRichTextComboBox


QSize QtRichTextComboBox::sizeHint ( ) const
{
	int	width	= 0,	height	= 0;
	for (int i = 0; i < count ( ); i++)
	{
		// Rem : icônes possibles non prises en compte ici.
		QTextDocument	doc;
		doc.setHtml (itemText (i));
		const QSize	size	= doc.size ( ).toSize ( );
		width	= width  >  size.width ( )  ? width  : size.width ( );
		height	= height >  size.height ( ) ? height : size.height ( );
	}	// for (int i = 0; i < count ( ); i++)

	int	left = 0, right	= 0, top = 0, bottom = 0;
	getContentsMargins (&left, &top, &right, &bottom);
	width	+= left + right;
	height	+= top + bottom;
	// Pour une raison non élucidée un deltax est esthétiquement souhaitable :
//	width	+= 2 * QtConfiguration::margin;

	// Issu de QComboBoxPrivate::recomputeSizeHint :
	QStyle*	cbStyle	= 0 == style ( ) ? QApplication::style ( ) : style ( );
	assert (0 != cbStyle);
	QSize	size (width, height);
	QStyleOptionComboBox	opt;
	initStyleOption (&opt);
	size	= cbStyle->sizeFromContents (QStyle::CT_ComboBox, &opt, size, this);
	size	= size.expandedTo (QApplication::globalStrut ( ));

	return size;
}	// sizeHint


QSize QtRichTextComboBox::minimumSizeHint ( ) const
{
	return sizeHint ( );
}	// minimumSizeHint


void QtRichTextComboBox::paintEvent (QPaintEvent*)
{
	// Le tour du widget : issu de QComboBox::paintEvent :
	QStylePainter stylePainter (this);
	stylePainter.setPen (palette ( ).color (QPalette::Text));

	// draw the combobox frame, focusrect and selected etc.
	QStyleOptionComboBox opt;
	initStyleOption (&opt);
	stylePainter.drawComplexControl (QStyle::CC_ComboBox, opt);

	// draw the icon and text :
	// Le texte : issu de QLabel::paintEvent
	QPainter	painter (this);
	QTextDocument	textDocument;
	textDocument.setHtml (currentText ( ));
	QRectF	rect (0, 0, 0, 0);
	rect.setSize (sizeHint ( ));
	int	left = 0, right	= 0, top = 0, bottom = 0;
	getContentsMargins (&left, &top, &right, &bottom);
	// Pour une raison non élucidée un deltax est esthétiquement
	// souhaitable (left et autres sont nuls) :
	painter.translate (left + QtConfiguration::margin, 0);
	textDocument.drawContents (&painter, rect);
}	// QtRichTextComboBox::paintEvent


QtComboBoxItemDelegate::QtComboBoxItemDelegate (QObject* parent)
	: QItemDelegate (parent)
{
}	// QtComboBoxItemDelegate::QtComboBoxItemDelegate


QSize QtComboBoxItemDelegate::sizeHint (
	 const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QString	text	= index.data (Qt::DisplayRole).toString ( );
	QTextDocument	textDocument;
	textDocument.setHtml (text);
	QSizeF	size	= textDocument.size ( );

	// Pour une raison non élucidée un deltax est esthétiquement souhaitable :
//	size	+= QSizeF(2 * QtConfiguration::margin, 0);

	return size.toSize ( );
}	// QtComboBoxItemDelegate::sizeHint


void QtComboBoxItemDelegate::paint (
				QPainter* painter, const QStyleOptionViewItem& option,
				const QModelIndex& index) const
{
	painter->save ( );

	// Première partie standard, selon QItemDelegate::paint :
	drawBackground (painter, option, index);
	drawFocus (painter, option, option.rect);

	// Seconde partie : on traite le texte comme étant du "rich text",
	// selon QLabel::paintEvent :
	QString	text	= index.data (Qt::DisplayRole).toString ( );
	QTextDocument	textDocument;
	textDocument.setHtml (text);
	painter->translate (0, option.rect.y ( ));
	painter->translate (option.rect.x ( ), 0);
	QRectF	rect (option.rect);
	rect.setY (0);
	textDocument.drawContents (painter, rect);

	painter->restore ( );
}	// QtComboBoxItemDelegate::paint

