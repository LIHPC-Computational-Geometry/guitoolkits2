#include "QtUtil/QtFontPanel.h"
#include "QtUtil/QtConfiguration.h"
#include <TkUtil/Exception.h>
#include <TkUtil/InternalError.h>

#include <QFont>
#ifndef QT_5
#include <QtGui/QLayout>
#include <QtGui/QColorDialog>
#else	// QT_5
#include <QLayout>
#include <QColorDialog>
#endif	// QT_5

#include <assert.h>


USING_UTIL
USING_STD


QtFontPanel::QtFontPanel (
			QWidget* parent, const QFont& font, const QColor& color,
			QtFontPanel::PANEL_ORIENTATION orientation)
	: QWidget (parent),
	  _color (color), _familyComboBox (0), _sizeTextField (0),
	  _boldCheckBox (0), _italicCheckBox (0), _underlineCheckBox (0)
{
	createGui (font, orientation);
}	// QtFontPanel::QtFontPanel


QtFontPanel::QtFontPanel (const QtFontPanel& panel)
	: QWidget (0), _color (panel._color),
	  _familyComboBox (0), _sizeTextField (0),
	  _boldCheckBox (0), _italicCheckBox (0), _underlineCheckBox (0)
{
	assert (0 && "QtFontPanel copy constructor is not allowed.");
}	// QtFontPanel::QtFontPanel (const QtFontPanel& view)


QtFontPanel& QtFontPanel::operator = (const QtFontPanel& view)
{
	assert (0 && "QtFontPanel::operator = is not allowed.");
	return *this;
}	// QtFontPanel::QtFontPanel (const QtFontPanel& view)


QtFontPanel::~QtFontPanel ( )
{
}	// QtFontPanel::~QtFontPanel


bool QtFontPanel::isColorEnabled ( ) const
{
	assert (0 != _colorButton);
	return _colorButton->isEnabled ( );
}	// QtFontPanel::isColorEnabled


void QtFontPanel::enableColor (bool enable)
{
	assert (0 != _colorButton);
	_colorButton->setEnabled (enable);
}	// QtFontPanel::enableColor


QFont QtFontPanel::getFont ( ) const
{
	assert (0 != _familyComboBox);
	QFont	font	= _familyComboBox->currentFont ( );
	font.setPointSize (getSize ( ));
	font.setBold (isBold ( ));
	font.setItalic (isItalic ( ));
	font.setUnderline (isUnderline ( ));

	return font;
}	// QtFontPanel::getFont


void QtFontPanel::setFont (const QFont& font)
{
	assert (0 != _familyComboBox);
	_familyComboBox->setCurrentFont (font);
	setSize (font.pointSize ( ));
	setBold (font.bold ( ));
	setItalic (font.italic ( ));
	setUnderline (font.underline ( ));
}	// QtFontPanel::setFont


unsigned int QtFontPanel::getSize ( ) const
{
	assert (0 != _sizeTextField);
	int	size	= _sizeTextField->getValue ( );
	if (0 > size)
	{
		UTF8String	msg;
		msg << "La taille de la fonte est négative (" << (long)size << ").";
		INTERNAL_ERROR (exc, msg, "QtFontPanel::getSize")
		throw exc;
	}	// if (0 > size)

	return (unsigned int)size;
}	// QtFontPanel::getSize


void QtFontPanel::setSize (unsigned int size)
{
	assert (0 != _sizeTextField);
	_sizeTextField->setValue (size);
}	// QtFontPanel::setSize


bool QtFontPanel::isBold ( ) const
{
	assert (0 != _boldCheckBox);
	return _boldCheckBox->isChecked ( );
}	// QtFontPanel::isBold


void QtFontPanel::setBold (bool bold)
{
	assert (0 != _boldCheckBox);
	_boldCheckBox->setChecked (bold);
}	// QtFontPanel::setBold


bool QtFontPanel::isItalic ( ) const
{
	assert (0 != _italicCheckBox);
	return _italicCheckBox->isChecked ( );
}	// QtFontPanel::isItalic


void QtFontPanel::setItalic (bool italic)
{
	assert (0 != _italicCheckBox);
	_italicCheckBox->setChecked (italic);
}	// QtFontPanel::setItalic


bool QtFontPanel::isUnderline ( ) const
{
	assert (0 != _underlineCheckBox);
	return _underlineCheckBox->isChecked ( );
}	// QtFontPanel::isUnderline


void QtFontPanel::setUnderline (bool underline)
{
	assert (0 != _underlineCheckBox);
	_underlineCheckBox->setChecked (underline);
}	// QtFontPanel::setUnderline


QColor QtFontPanel::getColor ( ) const
{
	return _color;
}	// QtFontPanel::getColor


void QtFontPanel::setColor (const QColor& color)
{
	_color	= color;
}	// QtFontPanel::setColor


void QtFontPanel::fontColorCallback ( )
{
	QColor	color	= QColorDialog::getColor (_color, this);
	if (true == color.isValid ( ))
		_color	= color;
}	// QtFontPanel::fontColorCallback


void QtFontPanel::createGui (
				const QFont& font, QtFontPanel::PANEL_ORIENTATION orientation)
{
	assert (0 == _familyComboBox);
	assert (0 == _sizeTextField);
	assert (0 == _boldCheckBox);
	assert (0 == _italicCheckBox);
	assert (0 == _underlineCheckBox);
	QBoxLayout*	mainLayout	= HORIZONTAL_PANEL == orientation ?
			(QBoxLayout*)new QHBoxLayout ( ) : (QBoxLayout*)new QVBoxLayout ( );

	// Famille / taille :
	QHBoxLayout*	hLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hLayout);
	_familyComboBox	= new QFontComboBox (this);
	_familyComboBox->setCurrentFont (font);
	hLayout->addWidget (_familyComboBox);
	hLayout->addSpacing (10);
	_sizeTextField	= new QtIntTextField (font.pointSize ( ), this);
	_sizeTextField->setRange (1, 200);
	hLayout->addWidget (_sizeTextField);

	if (HORIZONTAL_PANEL == orientation)
		hLayout->addSpacing (10);
	else
	{
		hLayout->addStretch (2);
		hLayout	= new QHBoxLayout ( );
		mainLayout->addLayout (hLayout);
	}	// else if (HORIZONTAL_PANEL == orientation)

	// Gras/Italique/Souligné/Couleur :
	_boldCheckBox	= new QCheckBox ("G", this);	
	QFont	cbFont	= _boldCheckBox->font ( );
	cbFont.setBold (true);
	_boldCheckBox->setFont (cbFont);
	_boldCheckBox->setChecked (font.bold ( ));
	hLayout->addWidget (_boldCheckBox);
	_italicCheckBox	= new QCheckBox ("I", this);	
	cbFont	= _italicCheckBox->font ( );
	cbFont.setItalic (true);
	_italicCheckBox->setFont (cbFont);
	_italicCheckBox->setChecked (font.italic ( ));
	hLayout->addWidget (_italicCheckBox);
	_underlineCheckBox	= new QCheckBox ("S", this);	
	cbFont	= _underlineCheckBox->font ( );
	cbFont.setUnderline (true);
	_underlineCheckBox->setFont (cbFont);
	_underlineCheckBox->setChecked (font.underline ( ));
	hLayout->addWidget (_underlineCheckBox);
	hLayout->addSpacing (10);
	_colorButton	= new QPushButton ("Couleur ...", this);
	connect (_colorButton, SIGNAL (clicked ( )), this,
	         SLOT (fontColorCallback ( )));
	hLayout->addWidget (_colorButton);
	hLayout->addStretch (2);

	setLayout (mainLayout);
}	// QtFontPanel::createGui

