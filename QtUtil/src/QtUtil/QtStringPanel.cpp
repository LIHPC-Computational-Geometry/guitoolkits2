#include "QtUtil/QtStringPanel.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"

#include <TkUtil/Exception.h>

#include <assert.h>

#ifndef QT_5
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#else	// QT_5
#include <QLayout>
#include <QLabel>
#endif	// QT_5

USING_STD
USING_UTIL

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtStringPanel::QtStringPanel (QWidget* parent, const UTF8String& title,
                              bool editable, const UTF8String& strTitle,
                              const UTF8String& value, QValidator* validator)
	: QtGroupBox (UTF8TOQSTRING (title), parent), 
	  _initialValue (value), _textField (0)
{
	QVBoxLayout*	layout	= new QVBoxLayout (this);
	setLayout (layout);
	setSpacing (QtConfiguration::spacing);
	setMargin (QtConfiguration::margin);
	QLabel*			label	= new QLabel (UTF8TOQSTRING (strTitle), this);
	label->setMinimumSize (label->sizeHint ( ));
	layout->addWidget (label);
	_textField				= new QtTextField (this);
	_textField->setMinimumSize (_textField->sizeHint ( ));
	layout->addWidget (_textField);

	_textField->setText (value);

	if (false == editable)
	{
		_textField->setReadOnly (true);
		setEnabled (false);
	}
	if (0 != validator)
		_textField->setValidator (validator);

	adjustSize ( );
}	// QtStringPanel::QtStringPanel


QtStringPanel::QtStringPanel (const QtStringPanel&)
	: QtGroupBox (0)
{
	assert (0 && "QtStringPanel copy constructor is forbidden.");
}	// QtStringPanel::QtStringPanel (const QtStringPanel&)


QtStringPanel& QtStringPanel::operator = (const QtStringPanel&)
{
	assert (0 && "QtStringPanel assignment operator is forbidden.");
	return *this;
}	// QtStringPanel::operator =


QtStringPanel::~QtStringPanel ( )
{
}	// QtStringPanel::~QtStringPanel


bool QtStringPanel::isModified ( ) const
{
	return getValue ( ) == _initialValue ? false : true;
}	// QtStringPanel::isModified


UTF8String QtStringPanel::getValue ( ) const
{
	assert ((0 != _textField) && "QtStringPanel::getValue : null text field.");
	return QtUnicodeHelper::qstringToUTF8String (_textField->text ( ));
}	// QtStringPanel::getValue


string QtStringPanel::getStdValue ( ) const
{
	assert ((0 != _textField) && "QtStringPanel::getStdValue : null text field.");
	return _textField->text ( ).toStdString ( );
}	// QtStringPanel::getStdValue



