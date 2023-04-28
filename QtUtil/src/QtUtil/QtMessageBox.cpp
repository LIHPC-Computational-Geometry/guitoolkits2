#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"

#include <TkUtil/Exception.h>

#include <iostream>
#include <stdexcept>
#include <assert.h>

#include <QCursor>
#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMessageBox>
#else	// QT_5
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#endif	// QT_5


USING_UTIL
USING_STD

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


#define BEGIN_TRY_CATCH_BLOCK                                              \
	try {

#define COMPLETE_TRY_CATCH_BLOCK                                           \
	}                                                                      \
	catch (const IN_UTIL Exception& exc)                                   \
	{                                                                      \
		QMessageBox::critical (0,                                          \
		             "ERREUR APPLICATIVE LORS DE L'AFFICHAGE D'UN MESSAGE",\
			UTF8TOQSTRING(exc.getFullMessage( )),\
			QMessageBox::Ok, QMessageBox::NoButton);                       \
	}                                                                      \
	catch (const IN_STD exception& exc)                                    \
	{                                                                      \
		QMessageBox::critical (0,                                          \
		             "ERREUR APPLICATIVE LORS DE L'AFFICHAGE D'UN MESSAGE",\
			QSTR (exc.what ( )),                                   \
			QMessageBox::Ok, QMessageBox::NoButton);                       \
	}                                                                      \
	catch (...)                                                            \
	{                                                                      \
		QMessageBox::critical (0,                                          \
		             "ERREUR APPLICATIVE LORS DE L'AFFICHAGE D'UN MESSAGE",\
			QSTR ("Erreur non documentée"),                                \
			QMessageBox::Ok, QMessageBox::NoButton);                       \
	}


// ===========================================================================
//                          LA CLASSE QtMessageDialog
// ===========================================================================


#include "QtUtil/QtConfiguration.h"

#ifndef QT_5
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#else	// QT_5
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#endif	// QT_5


QtMessageDialog::QtMessageDialog (
	QMessageBox::Icon icon, QWidget* parent,
	const QString& title, const QString& text, int columns,
	const char* button1, const char* button2, const char* button3,
	int defaultButton)
	: QDialog (parent)
{	
	setWindowTitle (title);

	QVBoxLayout*	mainLayout	= new QVBoxLayout (this);
	mainLayout->setSpacing (QtConfiguration::spacing);
	mainLayout->setMargin (QtConfiguration::margin);

	QHBoxLayout*	hboxLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hboxLayout);
	QLabel*			iconLabel	= new QLabel (this);
	iconLabel->setPixmap (QMessageBox::standardIcon (icon));
	hboxLayout->addWidget (iconLabel);
	QString	formattedText	=
		UTF8TOQSTRING (
				QtUnicodeHelper::qstringToUTF8String (text).format (columns));
	QTextEdit*	textEdit	= new QTextEdit (this);
	textEdit->setReadOnly (true);
	textEdit->setLineWrapMode (QTextEdit::NoWrap);
	textEdit->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	textEdit->setFrameStyle (QTextEdit::NoFrame);
	textEdit->setText (formattedText);
	textEdit->setMinimumSize (textEdit->sizeHint ( ));
	textEdit->setAutoFillBackground (true);
	textEdit->viewport ( )->setBackgroundRole (QPalette::NoRole);
	hboxLayout->addWidget (textEdit);

	hboxLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hboxLayout);
	_buttons [0]	= 0 == button1 ? 0 : new QPushButton (button1, this);
	_buttons [1]	= 0 == button2 ? 0 : new QPushButton (button2, this);
	_buttons [2]	= 0 == button3 ? 0 : new QPushButton (button3, this);
	hboxLayout->addStretch (200);
	for (int i = 0; i < 3; i++)
		if (0 != _buttons [i])
		{
			hboxLayout->addWidget (_buttons [i]);
			connect (_buttons [i], SIGNAL(clicked ( )), this,
			         SLOT (buttonClicked ( )));
			_buttons [i]->setDefault (defaultButton == i ? true : false);
			_buttons [i]->setAutoDefault (defaultButton == i ? true : false);
		}
	hboxLayout->addStretch (200);

	setLayout (mainLayout);
	mainLayout->activate ( );
	// Qt considère qu'on est dans une fenêtre scrollée et donc que la largeur
	// de cette fenêtre importe peu.
	// On force donc ici à prendre la largeur idéale du document :
	int	width	= (int)textEdit->document ( )->idealWidth ( );
	width	+= 2 * QtConfiguration::margin;
	int	height	= (int)textEdit->document ( )->size ( ).height ( );
	height	+= 2 * QtConfiguration::margin;
	// Les dimensions calculées sont elles acceptables ?
	QDesktopWidget*	desktop	= QApplication::desktop ( );
	if (0 != desktop)
	{
		const QRect	screenSize	= desktop->availableGeometry (QCursor::pos ( ));
		if (2 * width > screenSize.width ( ))
			width	= screenSize.width ( ) / 2;
		if (2 * height > screenSize.height ( ))
			height	= screenSize.height ( ) / 2;
	}	// if (0 != desktop)
	textEdit->setMinimumSize (QSize (width, height));
	textEdit->setFixedSize (QSize (width, height));
	setMinimumSize (sizeHint ( ));
}	// QtMessageDialog::QtMessageDialog


QtMessageDialog::QtMessageDialog (const QtMessageDialog&)
{
	assert (0 && "QtMessageDialog copy constructor is not allowed.");
}	// QtMessageDialog::QtMessageDialog


QtMessageDialog& QtMessageDialog::operator = (const QtMessageDialog&)
{
	assert (0 && "QtMessageDialog operator = is not allowed.");
	return *this;
}	// QtMessageDialog::operator =


QtMessageDialog::~QtMessageDialog ( )
{
}	// QtMessageDialog::~QtMessageDialog


void QtMessageDialog::buttonClicked ( )
{
	const QObject*		s	= sender ( );
	for (int i = 0; i < 3; i++)
		if (s == _buttons [i])
		{
			done (i);
			return;
		}	// if (s == _buttons [i])
}	// QtMessageDialog::buttonClicked



// ===========================================================================
//                           LA CLASSE QtMessageBox
// ===========================================================================

QtMessageBox::QtMessageBox ( )
{
	assert (0 && "QtMessageBox::QtMessageBox is not allowed.");
}	// QtMessageBox::QtMessageBox


QtMessageBox::QtMessageBox (const QtMessageBox&)
{
	assert (0 && "QtMessageBox::QtMessageBox is not allowed.");
}	// QtMessageBox::QtMessageBox


QtMessageBox& QtMessageBox::operator = (const QtMessageBox&)
{
	assert (0 && "QtMessageBox::operator = is not allowed.");
	return *this;
}	// QtMessageBox::operator =


QtMessageBox::~QtMessageBox ( )
{
	assert (0 && "QtMessageBox::~QtMessageBox is not allowed.");
}	// QtMessageBox::~QtMessageBox


void QtMessageBox::displayInformationMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum)
{
	displayInformationMessage (parent, title, message, columnNum, "OK");
}	// QtMessageBox::displayInformationMessage


int QtMessageBox::displayInformationMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum,
				const char* button0Text, const char* button1Text,
				const char* button2Text, int defaultButtonNumber
		)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (
		QMessageBox::Information, parent, 
		UTF8TOQSTRING (title),
		UTF8TOQSTRING (message), columnNum,
		button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );
	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayInformationMessage


void QtMessageBox::displayWarningMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum)
{
	displayWarningMessage (parent, title, message, columnNum, "OK");
}	// QtMessageBox::displayWarningMessage


int QtMessageBox::displayWarningMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum,
				const char* button0Text, const char* button1Text,
				const char* button2Text, int defaultButtonNumber
		)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (
		QMessageBox::Warning, parent, 
		UTF8TOQSTRING (title),
		UTF8TOQSTRING (message), columnNum,
		button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayWarningMessage


void QtMessageBox::displayErrorMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum)
{
	displayErrorMessage (parent, title, message, columnNum, "OK");
}	// QtMessageBox::displayErrorMessage


int QtMessageBox::displayErrorMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum,
				const char* button0Text, const char* button1Text,
				const char* button2Text, int defaultButtonNumber
		)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (
		QMessageBox::Critical, parent, 
		UTF8TOQSTRING (title),
		UTF8TOQSTRING (message), columnNum,
		button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayErrorMessage


int QtMessageBox::displayQuestionMessage (
				QWidget* parent, const UTF8String& title, 
				const UTF8String& message, size_t columnNum,
				const char* button0Text, const char* button1Text,
				const char* button2Text, int defaultButtonNumber
		)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (
		QMessageBox::Question, parent, 
		UTF8TOQSTRING (title),
		UTF8TOQSTRING (message), columnNum,
		button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayQuestionMessage


