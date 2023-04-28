#include "QtUtil/QtCharsetedFileDialog.h"

#include <TkUtil/InternalError.h>
#include <TkUtil/MemoryError.h>
#include <QtUtil/QtUnicodeHelper.h>

#include <QBoxLayout>

#include <assert.h>


USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtCharsetedFileDialog::QtCharsetedFileDialog (
	QWidget* parent, const UTF8String& title, Charset::CHARSET cs)
	: QFileDialog (parent, UTF8TOQSTRING (title)),
	  _charsetComboBox (0), _extensionWidget (0)
{
	// ! native sinon plantage sous Cent OS 7 / Gnome / Qt 5.9.1
	setOption (QFileDialog::DontUseNativeDialog);
	createExtension (cs);

	adjustSize ( );
}	// QtCharsetedFileDialog::QtCharsetedFileDialog


QtCharsetedFileDialog::QtCharsetedFileDialog (
			QWidget* parent, const UTF8String& title, const string& directory,
			Charset::CHARSET cs)
	: QFileDialog (parent, UTF8TOQSTRING (title), directory.c_str ( )),
	  _charsetComboBox (0), _extensionWidget (0)
{
	// ! native sinon plantage sous Cent OS 7 / Gnome / Qt 5.9.1
	setOption (QFileDialog::DontUseNativeDialog);
	createExtension (cs);

	adjustSize ( );
}	// QtCharsetedFileDialog::QtCharsetedFileDialog


QtCharsetedFileDialog::QtCharsetedFileDialog (
	QWidget* parent, const UTF8String& title,
	const vector<Charset::CHARSET>& charsets, Charset::CHARSET cs)
	: QFileDialog (parent, UTF8TOQSTRING (title)),
	  _charsetComboBox (0), _extensionWidget (0)
{
	// ! native sinon plantage sous Cent OS 7 / Gnome / Qt 5.9.1
	setOption (QFileDialog::DontUseNativeDialog);
	createExtension (charsets, cs);

	adjustSize ( );
}	// QtCharsetedFileDialog::QtCharsetedFileDialog


QtCharsetedFileDialog::QtCharsetedFileDialog (
			QWidget* parent, const UTF8String& title, const string& directory,
			const vector<Charset::CHARSET>& charsets, Charset::CHARSET cs)
	: QFileDialog (parent, UTF8TOQSTRING (title), directory.c_str ( )),
	  _charsetComboBox (0), _extensionWidget (0)
{
	// ! native sinon plantage sous Cent OS 7 / Gnome / Qt 5.9.1
	setOption (QFileDialog::DontUseNativeDialog);
	createExtension (charsets, cs);

	adjustSize ( );
}	// QtCharsetedFileDialog::QtCharsetedFileDialog


QtCharsetedFileDialog::QtCharsetedFileDialog (const QtCharsetedFileDialog&)
	: QFileDialog (0), _charsetComboBox (0), _extensionWidget (0)
{
	assert (0 && "QtCharsetedFileDialog copy constructor is forbidden.");
}	// QtCharsetedFileDialog::QtCharsetedFileDialog (const QtCharsetedFileDialog&)


QtCharsetedFileDialog& QtCharsetedFileDialog::operator = (
											const QtCharsetedFileDialog&)
{
	assert (0 && "QtCharsetedFileDialog assignment operator is forbidden.");
	return *this;
}	// QtCharsetedFileDialog::operator =


QtCharsetedFileDialog::~QtCharsetedFileDialog ( )
{
}	// QtCharsetedFileDialog::~QtCharsetedFileDialog


Charset::CHARSET QtCharsetedFileDialog::getCharset ( ) const
{
	CHECK_NULL_PTR_ERROR (_charsetComboBox)
	return _charsetComboBox->getCharset ( );
}	// QtCharsetedFileDialog::getCharset


const QtCharsetComboBox& QtCharsetedFileDialog::getCharsetComboBox ( ) const
{
	CHECK_NULL_PTR_ERROR (_charsetComboBox)
	return _charsetComboBox->getComboBox ( );
}	// QtCharsetedFileDialog::getCharsetComboBox


QtCharsetComboBox& QtCharsetedFileDialog::getCharsetComboBox ( )
{
	CHECK_NULL_PTR_ERROR (_charsetComboBox)
	return _charsetComboBox->getComboBox ( );
}	// QtCharsetedFileDialog::getCharsetComboBox


void QtCharsetedFileDialog::createExtension (Charset::CHARSET cs)
{
	_extensionWidget	= new QWidget ( );
	QVBoxLayout*	mainLayout	= new QVBoxLayout ( );
	_extensionWidget->setLayout (mainLayout);
	_charsetComboBox		=
			new QtLabelisedCharsetComboBox (
						_extensionWidget, UTF8STR ("Jeu de caractères :"), cs);
	mainLayout->addWidget (_charsetComboBox);
	
	QLayout*	dlgLayout	= layout ( );
	if (0 != dlgLayout)
	{
		QGridLayout*	gridLayout	= dynamic_cast<QGridLayout*>(dlgLayout);
		if (0 != gridLayout)
			gridLayout->addWidget (
						_extensionWidget, gridLayout->rowCount( ), 0, 1, 2);
		else
			layout ( )->addWidget (_extensionWidget);
	}	// if (0 != dlgLayout)
}	// QtCharsetedFileDialog::createExtension


void QtCharsetedFileDialog::createExtension (
				const vector<Charset::CHARSET>& charsets, Charset::CHARSET cs)
{
	_extensionWidget	= new QWidget ( );
	QVBoxLayout*	mainLayout	= new QVBoxLayout ( );
	_extensionWidget->setLayout (mainLayout);
	_charsetComboBox		=
			new QtLabelisedCharsetComboBox (
				_extensionWidget, UTF8STR("Jeu de caractères :"), charsets, cs);
	mainLayout->addWidget (_charsetComboBox);
	
	QLayout*	dlgLayout	= layout ( );
	if (0 != dlgLayout)
	{
		QGridLayout*	gridLayout	= dynamic_cast<QGridLayout*>(dlgLayout);
		if (0 != gridLayout)
			gridLayout->addWidget (
						_extensionWidget, gridLayout->rowCount( ), 0, 1, 2);
		else
			layout ( )->addWidget (_extensionWidget);
	}	// if (0 != dlgLayout)
}	// QtCharsetedFileDialog::createExtension


QWidget& QtCharsetedFileDialog::getExtension ( )
{
	if (0 == _extensionWidget)
		throw Exception ("QtCharsetedFileDialog::getExtension : null extension widget.");
	return *_extensionWidget;
}	// QtCharsetedFileDialog::getExtension
