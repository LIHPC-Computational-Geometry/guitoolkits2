#include "QtUtil/QtExtMessageBox.h"
#include "QtUtil/QtConfiguration.h"

#include "QtUtil/QtUnicodeHelper.h"
static const TkUtil::Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION

#ifndef QT_5
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#else	// QT_5
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#endif	// QT_5

#include <assert.h>
#include <iostream>

using namespace TkUtil;


QtExtMessageBox::QtExtMessageBox (const char* title, 
                                  QWidget* parent,
                                  const char* text,
                                  const char* button1, const char* button2,
                                  const char* button3, const char* checkBox,
                                  QMessageBox::Icon icon, int defaultButton,
                                  bool defaultCheckBox)
	: QDialog (parent),
	  _checkBox (0)
{
	Charset	iso (Charset::ISO_8859);
	setWindowTitle (title);
	createGui (
			UTF8String (text, iso), UTF8String (button1, iso),
			UTF8String (button2, iso), UTF8String (button3, iso),
			UTF8String (checkBox, iso), icon, defaultButton, defaultCheckBox);
}	// QtExtMessageBox::QtExtMessageBox


QtExtMessageBox::QtExtMessageBox (
		const UTF8String& title, QWidget* parent, const UTF8String& text,
		const UTF8String& button1, const UTF8String& button2,
		const UTF8String& button3, const UTF8String& checkBox,
		QMessageBox::Icon icon, int defaultButton, bool defaultCheckBox)
	: QDialog (parent),
	  _checkBox (0)
{
	setWindowTitle (UTF8TOQSTRING (title));
	createGui (text, button1, button2, button3, checkBox, icon, defaultButton,
	           defaultCheckBox);
}	// QtExtMessageBox::QtExtMessageBox


QtExtMessageBox::QtExtMessageBox (const QtExtMessageBox&)
{
	assert (0 && "QtExtMessageBox::QtExtMessageBox (const QtExtMessageBox&) is not allowed.");
}	// QtExtMessageBox::QtExtMessageBox (const QtExtMessageBox&)


QtExtMessageBox& QtExtMessageBox::operator = (const QtExtMessageBox&)
{
	assert (0 && "QtExtMessageBox::operator = is not allowed.");
	return *this;
}	// QtExtMessageBox::operator = 


QtExtMessageBox::~QtExtMessageBox ( )
{
}	// QtExtMessageBox::~QtExtMessageBox


void QtExtMessageBox::createGui (
		const UTF8String& text, const UTF8String& button1,
		const UTF8String& button2, const UTF8String& button3,
		const UTF8String& checkBox, QMessageBox::Icon icon,
		int defaultButton, bool defaultCheckBox)
{
	QWidget*		vbox		= new QWidget (this);
	QVBoxLayout*	vboxLayout	= new QVBoxLayout ( );
	vbox->setLayout (vboxLayout);

	// L'intitule :
	QWidget*		labelsbox		= new QWidget (vbox);
	QHBoxLayout*	labelsLayout	= new QHBoxLayout ( );
	labelsbox->setLayout (labelsLayout);
	vboxLayout->addWidget (labelsbox);
	labelsLayout->setSpacing (QtConfiguration::spacing);
	labelsLayout->setMargin (QtConfiguration::margin);
	QLabel*	iconLabel	= new QLabel (labelsbox);
	iconLabel->setPixmap (QMessageBox::standardIcon (icon));
	QLabel*	label	= new QLabel (UTF8TOQSTRING (text), labelsbox);
	labelsLayout->addWidget (iconLabel);
	labelsLayout->addWidget (label);

	// La case a cocher :
	_checkBox	= new QCheckBox (UTF8TOQSTRING (checkBox), vbox);
	_checkBox->setChecked (defaultCheckBox);
	vboxLayout->addWidget (_checkBox);

	// Les boutons :
	QWidget*		buttonsbox		= new QWidget (vbox);
	QHBoxLayout*	buttonsLayout	= new QHBoxLayout ( );
	buttonsbox->setLayout (buttonsLayout);
	vboxLayout->addWidget (buttonsbox);
	buttonsLayout->setSpacing (QtConfiguration::spacing);
	buttonsLayout->setMargin (QtConfiguration::margin);
	_buttons [0]	= true == button1.empty ( ) ?
				0 : new QPushButton (UTF8TOQSTRING (button1), buttonsbox);
	_buttons [1]	= true == button2.empty ( ) ?
				0 : new QPushButton (UTF8TOQSTRING (button2), buttonsbox);
	_buttons [2]	= true == button3.empty ( ) ?
				0 : new QPushButton (UTF8TOQSTRING (button3), buttonsbox);
	buttonsLayout->addStretch (200);
	for (int j = 0; j < 3; j++)
		if (0 != _buttons [j])
			buttonsLayout->addWidget (_buttons [j]);
	buttonsLayout->addStretch (200);
	for (int i = 0; i < 3; i++)
		if (0 != _buttons [i])
			connect (_buttons [i], SIGNAL(clicked ( )), this, 
			         SLOT (buttonClicked ()));
	if ((1 <= defaultButton) && (3 >= defaultButton) &&
	    (0 != _buttons [defaultButton - 1]))
		_buttons [defaultButton - 1]->setDefault (true);

	vboxLayout->setSpacing (QtConfiguration::spacing);
	vboxLayout->setMargin (QtConfiguration::margin);
	vbox->setFixedSize (vbox->sizeHint ( ));
}	// QtExtMessageBox::createGui


bool QtExtMessageBox::getCheckBoxValue ( ) const
{
	return _checkBox->isChecked ( );
}	// QtExtMessageBox::getCheckBoxValue


void QtExtMessageBox::buttonClicked ( )
{
	const QObject*	s	= sender ( );
	for (int i = 0; i < 3; i++)
		if (s == _buttons [i])
		{
			done (i + 1);
			return;
		}
}	// QtExtMessageBox::buttonClicked
