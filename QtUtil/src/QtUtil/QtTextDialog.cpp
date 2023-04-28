#include "QtUtil/QtTextDialog.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/QtGroupBox.h"

#ifndef QT_5
#include <QtGui/QLabel>
#else	// QT_5
#include <QLabel>
#endif	// QT_5

#include <assert.h>



USING_STD
USING_UTIL


QtTextDialog::QtTextDialog (
			QWidget* parent, const UTF8String& title, 
			const UTF8String& text, bool editable)
	: QDialog (parent, QtConfiguration::modalDialogWFlags),
	  _centralLayout (0), _textWindow (0), _closurePanel (0), _modified (false)
{	
	createGui (title, text, editable);
}	// QtTextDialog::QtTextDialog


QtTextDialog::QtTextDialog (
	QWidget* parent, const UTF8String& title, const UTF8String& text,
	bool editable, const QFont& font, const QColor& foreground,
	const QColor& background)
{
	createGui (title, "", editable);
	assert (0 != _textWindow);
	_textWindow->setFont (font);
	_textWindow->setTextColor (foreground);
	_textWindow->setAutoFillBackground (true);
	QPalette	palette (_textWindow->palette ( ));
//	palette.setColor (QPalette::Active, QPalette::Background, background);
//	palette.setColor (QPalette::Inactive, QPalette::Background, background);
	palette.setColor (QPalette::Active, QPalette::Base, background);
	palette.setColor (QPalette::Inactive, QPalette::Base, background);
//	palette.setColor (QPalette::Active, QPalette::Window, background);
//	palette.setColor (QPalette::Inactive, QPalette::Window, background);
	_textWindow->setPalette (palette);
/* OK et suffisant
	if (0 != _textWindow->viewport ( ))
	{
		QPalette	palette (_textWindow->viewport ( )->palette ( ));
		palette.setColor (QPalette::Active, QPalette::Base, background);
		palette.setColor (QPalette::Inactive, QPalette::Base, background);
		_textWindow->viewport ( )->setPalette (palette);
	}	// if (0 != _textWindow->viewport ( ))
*/
	_textWindow->setText (QtUnicodeHelper::unicodeStringToQString (text));
}	// QtTextDialog::QtTextDialog


QtTextDialog::QtTextDialog (const QtTextDialog&)
{
	assert (0 && "QtTextDialog copy constructor is forbidden.");
}	// QtTextDialog::QtTextDialog (const QtTextDialog&)


QtTextDialog& QtTextDialog::operator = (const QtTextDialog&)
{
	assert (0 && "QtTextDialog assignment operator is forbidden.");
	return *this;
}	// QtTextDialog::operator =


QtTextDialog::~QtTextDialog ( )
{
}	// QtTextDialog::~QtTextDialog


UTF8String QtTextDialog::getText ( ) const
{	
	assert (0 != _textWindow);
	return QtUnicodeHelper::qstringToUTF8String (_textWindow->toPlainText( ));
}	// QtTextDialog::getText 


QString QtTextDialog::getQText ( ) const
{
	assert (0 != _textWindow);
	return _textWindow->toPlainText( );
}	// QtTextDialog::getQText


void QtTextDialog::textModified ( )
{
	assert (0 != _closurePanel);
	assert (0 != _closurePanel->getApplyButton ( ));
	assert (0 != _closurePanel->getCancelButton ( ));
	_modified	= true;
	_closurePanel->getApplyButton ( )->setEnabled (true);
	_closurePanel->getCancelButton ( )->setText ("Annuler");
}	// QtTextDialog::textModified


void QtTextDialog::apply ( )
{
	QDialog::accept ( );
	emit applied (this);
}	// QtTextDialog::apply


void QtTextDialog::reject ( )
{
	QDialog::reject ( );
	emit canceled (this);
}	// QtTextDialog::reject


const QVBoxLayout& QtTextDialog::getCentralLayout ( ) const
{
	assert (0 != _centralLayout);
	return *_centralLayout;
}	// QtTextDialog::getCentralLayout


QVBoxLayout& QtTextDialog::getCentralLayout ( )
{
	assert (0 != _centralLayout);
	return *_centralLayout;
}	// QtTextDialog::getCentralLayout


const QTextEdit& QtTextDialog::getTextEditor ( ) const
{
	assert (0 != _textWindow);
	return *_textWindow;
}	// QtTextDialog::getTextEditor


QTextEdit& QtTextDialog::getTextEditor ( )
{
	assert (0 != _textWindow);
	return *_textWindow;
}	// QtTextDialog::getTextEditor


void QtTextDialog::createGui (
		const UTF8String & title, const UTF8String & text, bool editable)
{
	setModal (true);
	setWindowTitle (UTF8TOQSTRING (title));

	// Creation de l'ihm :
	QVBoxLayout*	layout	= new QVBoxLayout (this);
	layout->setMargin (QtConfiguration::margin);
	layout->setSizeConstraint (QLayout::SetMinimumSize);
	_centralLayout	= new QVBoxLayout ( );
	_centralLayout->setSizeConstraint (QLayout::SetMinimumSize);
	layout->addLayout (_centralLayout);
	_textWindow	= new QTextEdit (this);
	_textWindow->setText (UTF8TOQSTRING (text));
	connect (_textWindow, SIGNAL(textChanged ( )), this, 
	         SLOT (textModified ( )));
	_centralLayout->addWidget (_textWindow);
	_textWindow->setMinimumSize (_textWindow->sizeHint ( ));

	layout->addWidget (new QLabel (" ", this));
	_closurePanel	= 
		new QtDlgClosurePanel (this, false, "Appliquer", "", "Fermer", "", "");
	layout->addWidget (_closurePanel);
	_closurePanel->setMinimumSize (_closurePanel->sizeHint ( ));

	// Par defaut le bouton OK est artificellement clique par QDialog quand
	// l'utilisateur fait return dans un champ de texte => on inhibe ce
	// comportement par defaut :
	_closurePanel->getApplyButton ( )->setAutoDefault (false);
	_closurePanel->getApplyButton ( )->setDefault (false);
	_closurePanel->getApplyButton ( )->setEnabled (false);
	_closurePanel->getCancelButton ( )->setAutoDefault (false);
	_closurePanel->getCancelButton ( )->setDefault (false);
	connect (_closurePanel->getApplyButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(apply ( )));
	connect (_closurePanel->getCancelButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(reject ( )));

	layout->activate ( );
	setMinimumSize (layout->sizeHint ( ));
	resize (640, 700);
}	// QtTextDialog::createGui

