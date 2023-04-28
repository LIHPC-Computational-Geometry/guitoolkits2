#include "QtUtil/QtColorChooser.h"
#include "QtUtil/QtUnicodeHelper.h"

#include <QColorDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include <assert.h>



USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION

QtColorChooser::QtColorChooser (
			QWidget* parent, const UTF8String& label, const Color& color)
	: QWidget (parent), _colorLabel (0)
{
	createGui (label, color);
}	// QtColorChooser::QtColorChooser


QtColorChooser::QtColorChooser (
			QWidget* parent, const UTF8String& label, const QColor& color)
	: QWidget (parent), _colorLabel (0)
{
	createGui (label, Color (color.red ( ), color.green ( ), color.blue ( )));
}	// QtColorChooser::QtColorChooser


QtColorChooser::QtColorChooser (const QtColorChooser&)
	: QWidget ( ), _colorLabel (0)
{
	assert (0 && "QtColorChooser copy constructor is forbidden.");
}	// QtColorChooser::QtColorChooser


QtColorChooser& QtColorChooser::operator = (const QtColorChooser&)
{
	assert (0 && "QtColorChooser assignment operator is forbidden.");
	return *this;
}	// QtColorChooser::operator =


QtColorChooser::~QtColorChooser ( )
{
}	// QtColorChooser::~QtColorChooser


void QtColorChooser::createGui (const UTF8String& label, const Color& color)
{
	QHBoxLayout*	layout	= new QHBoxLayout ( );
	setLayout (layout);
	QPushButton*	button	= new QPushButton (UTF8TOQSTRING (label), this);
	button->setAutoDefault (false);
	button->setDefault (false);
	layout->addWidget (button);
	_colorLabel	= new QLabel ("          ", this);
	_colorLabel->setAutoFillBackground (true);
	QPalette	palette (_colorLabel->palette ( ));
	QColor		qcolor (color.getRed( ), color.getGreen( ), color.getBlue( ));
	palette.setBrush (QPalette::Background, qcolor);
	_colorLabel->setPalette (palette);
	layout->addWidget (_colorLabel );

	connect (button, SIGNAL (clicked (bool)), this, SLOT (clickedCallback ( )));
}	// QtColorChooser::createGui


QColor QtColorChooser::getQColor ( ) const
{
	assert ((0 != _colorLabel) && "QtColorChooser::getQColor : null color label.");
	QPalette	palette	= _colorLabel->palette ( );

	return palette.window ( ).color ( );
}	// QtColorChooser::getQColor


void QtColorChooser::setQColor (const QColor& color)
{
	assert ((0 != _colorLabel) && "QtColorChooser::setQColor : null color label.");
	QPalette	palette (_colorLabel->palette ( ));
	palette.setBrush (QPalette::Background, color);
	_colorLabel->setPalette (palette);
}	// QtColorChooser::setQColor


Color QtColorChooser::getColor ( ) const
{
	QColor	qcolor	= getQColor ( );

	return Color (qcolor.red ( ), qcolor.green ( ), qcolor.blue ( ));
}	// QtColorChooser::getQColor


void QtColorChooser::setColor (const Color& color)
{
	setQColor (QColor (color.getRed ( ), color.getGreen ( ), color.getBlue ( )));
}	// QtColorChooser::setColor


void QtColorChooser::clickedCallback ( )
{
	assert ((0 != _colorLabel) && "QtColorChooser::clickedCallback : null color label.");
	const QColor	current	= getQColor ( );
	const QColor	color	= QColorDialog::getColor (getQColor ( ), this,
				"Nouvelle couleur", QColorDialog::DontUseNativeDialog);
	if (false == color.isValid ( ))
		return;

	if (current == color)
		return;

	QPalette	palette (_colorLabel->palette ( ));
	palette.setBrush (QPalette::Background, color);
	_colorLabel->setPalette (palette);

	emit (colorChanged (color));
}	// QtColorChooser::clickedCallback


