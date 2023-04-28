#include "QtUtil/QtTextField.h"

#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QStyleOptionFrameV2>
#else	// QT_5
#include <QApplication>
#include <QStyleOptionFrameV2>
#endif	// QT_5

#include <assert.h>
#include <iostream>


USING_STD


QtTextField::QtTextField (QWidget* parent, const char* name)
	: QLineEdit (parent)
{
}	// QtTextField::QtTextField


QtTextField::QtTextField (const QString& contents, QWidget* parent, 
                          const char* name)
	: QLineEdit (contents, parent)
{
}	// QtTextField::QtTextField


QtTextField::QtTextField (const QtTextField&)
	: QLineEdit (0, 0)
{
	assert (0 && "QtTextField::QtTextField is not allowed.");
}	// QtTextField::QtTextField (const QtTextField&)


QtTextField& QtTextField::operator = (const QtTextField&)
{
	assert (0 && "QtTextField::operator = is not allowed.");
	return *this;
}	// QtTextField::operator =


QtTextField::~QtTextField ( )
{
}	// QtTextField::~QtTextField


string QtTextField::getText ( ) const
{
	return text ( ).toStdString ( );
}	// QtTextField::getText


QString QtTextField::getQText ( ) const
{
	return text ( );
}	// QtTextField::getQText


void QtTextField::setText (const string& text)
{
	QLineEdit::setText (text.c_str ( ));
}	// QtTextField::setText


void QtTextField::setText (const QString& text)
{
	QLineEdit::setText (text);
}	// QtTextField::setText


void QtTextField::setVisibleColumns (int count)
{
	const int	length	= maxLength ( );
	setMaxLength (count);
	setFixedSize (sizeHint ( ));
	setMaxLength (length);
}	// QtTextField::setVisibleColumns


QSize QtTextField::sizeHint ( ) const
{	// L'algo par défaut (cf. QLineEdit::sizeHint de Qt 4.7.4) semble
	// pré-programmé pour un champ de 17 caractères visibles.
	// QLineEdit::minimumSizeHint retourne la dimension pour un caractère,
	// dimension qui contient également les marges.
	const int	length	= maxLength ( );
	if (32767 == length)
		return QLineEdit::sizeHint ( );

	int		ltm 		= 0, rtm = 0, ttm = 0, btm = 0;
	int		lcm 		= 0, rcm = 0, tcm = 0, bcm = 0;
	getTextMargins (&ltm, &ttm, &rtm, &btm);
	getContentsMargins (&lcm, &tcm, &rcm, &bcm);

	// Code issu des sources de QLineEdit::minimumSizeHint de Qt 4.7.4 :
	QFontMetrics	fm	= fontMetrics ( );
//	const int		charWidth	= true == isNumeric ( ) ? fm.width ('0') : fm.maxWidth ( ); // v 5.2.1
    const int		charWidth	= true == isNumeric ( ) ? fm.width ('0') : fm.averageCharWidth ( ); // v 5.2.1 (largeur moyenne = 9, largeur max = 53 ...)
	int	h	= fm.height ( ) + qMax (ttm + btm, fm.leading ( )) + tcm + bcm;
	int	w	= length * charWidth + lcm + rcm;
	if (true == isNumeric ( ))
		w	+= 6;	// Calcul un peu juste si marges nulles ...
    // v 5.2.1 : on essaie de s'approcher au max de la requête demandée, sans tomber dans des excès.
    // => s'il y a des caractères on prend les maxLengths premiers et on retient la taille qui leur est nécessaire.
    QString currentText = text ( ).left (maxLength ( ));
	QSize   currentSize = fm.boundingRect (currentText).size ( );
	w   = w > currentSize.width ( ) ? w : currentSize.width ( );    // v 5.2.1

    QStyleOptionFrameV2	opt;
    initStyleOption (&opt);
    return style ( )->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h).expandedTo (QApplication::globalStrut ( )), this);
}	// QtTextField::sizeHint


bool QtTextField::isNumeric ( ) const
{
	return false;
}	// QtTextField::isNumeric
