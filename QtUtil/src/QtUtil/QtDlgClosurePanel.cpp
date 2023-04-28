#include "QtUtil/QtDlgClosurePanel.h"
#include "QtUtil/QtHelpWindow.h"

#include <assert.h>

#ifndef QT_5
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#else	// QT_5
#include <QHBoxLayout>
#include <QLabel>
#endif	// QT_5


USING_STD


QtDlgClosurePanel::QtDlgClosurePanel (QWidget* parent, bool hasClose,
                                      const string& button1,
                                      const string& button2,
                                      const string& button3,
                                      const string& helpURL,
                                      const string& helpTag)
	: QWidget (parent),
	  _applyButton (0), _closeButton (0), _cancelButton (0),
	  _helpButton (0), _helpURL (helpURL), _helpTag (helpTag)
{
	QHBoxLayout*	hboxLayout	= new QHBoxLayout (this);
	hboxLayout->setSpacing (10);

	// Creation de l'ihm :
	QLabel*	label1	= 0, *label2	= 0, *label3	= 0,
			*label4	= 0, *label5	= 0;
	label1	= new QLabel (" ", this);
	_applyButton	= new QPushButton (button1.c_str ( ), this);
	if (true == hasClose)
	{
		label2	= new QLabel (" ", this);
		_closeButton	= new QPushButton (button2.c_str ( ), this);
	}	// if (true == hasClose)
	if (0 != button3.length ( ))
	{
		label3	= new QLabel (" ", this);
		_cancelButton	= new QPushButton (button3.c_str ( ), this);
	}

	if (0 != _helpURL.length ( ))
	{
		label4	= new QLabel (" ", this);
		_helpButton	= new QPushButton ("Aide", this);
		connect (_helpButton, SIGNAL (clicked ( )), this, 
		          SLOT (displayHelp ( )));
		_helpButton->setAutoDefault (false);
		_helpButton->setDefault (false);
	}	// if (0 != _helpURL.length ( ))
	label5	= new QLabel (" ", this);
	assert (0 != hboxLayout);
	if (0 != label1)		hboxLayout->addWidget (label1);
	if (0 != _applyButton)	hboxLayout->addWidget (_applyButton);
	if (0 != label2)		hboxLayout->addWidget (label2);
	if (0 != _closeButton)	hboxLayout->addWidget (_closeButton);
	if (0 != label3)		hboxLayout->addWidget (label3);
	if (0 != _cancelButton)	hboxLayout->addWidget (_cancelButton);
	if (0 != label4)		hboxLayout->addWidget (label4);
	if (0 != _helpButton)	hboxLayout->addWidget (_helpButton);
	if (0 != label5)		hboxLayout->addWidget (label5);
//	setLayout (hboxLayout);

	setMinimumSize (sizeHint ( ));
}	// QtDlgClosurePanel::QtDlgClosurePanel


QtDlgClosurePanel::QtDlgClosurePanel (const QtDlgClosurePanel&)
{
	assert (0 && "QtDlgClosurePanel copy constructor is forbidden.");
}	// QtDlgClosurePanel::QtDlgClosurePanel (const QtDlgClosurePanel&)


QtDlgClosurePanel& QtDlgClosurePanel::operator = (const QtDlgClosurePanel&)
{
	assert (0 && "QtDlgClosurePanel assignment operator is forbidden.");
	return *this;
}	// QtDlgClosurePanel::operator =


QtDlgClosurePanel::~QtDlgClosurePanel ( )
{
}	// QtDlgClosurePanel::~QtDlgClosurePanel


QPushButton* QtDlgClosurePanel::getApplyButton ( ) const
{
	return _applyButton;
}	// QtDlgClosurePanel::getApplyButton


QPushButton* QtDlgClosurePanel::getCloseButton ( ) const
{
	return _closeButton;
}	// QtDlgClosurePanel::getCloseButton


QPushButton* QtDlgClosurePanel::getCancelButton ( ) const
{
	return _cancelButton;
}	// QtDlgClosurePanel::getCancelButton


QPushButton* QtDlgClosurePanel::getHelpButton ( ) const
{
	return _helpButton;
}	// QtDlgClosurePanel::getHelpButton


void QtDlgClosurePanel::displayHelp ( )
{
	QtHelpWindow::displayURL (_helpURL, _helpTag);
}	// QtDlgClosurePanel::displayHelp


