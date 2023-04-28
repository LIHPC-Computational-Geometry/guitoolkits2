#include "QtUtil/QtAboutDialog.h"
#include "QtUtil/QtConfiguration.h"

#include <TkUtil/UTF8String.h>

#include "QtUtil/QtUnicodeHelper.h"

#ifndef QT_5
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#endif	// QT_5

#include <assert.h>
#include <iostream>

using namespace TkUtil;
using namespace std;

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtAboutDialog::QtAboutDialog (
	QWidget* parent, const UTF8String& title, const string& version,
	const string& url, const UTF8String& description)
	: QDialog (parent),
	  _layout (0), _name (title), _version (version), _url (url),
	  _description (description)
{
	UTF8String	dlgTitle;
	dlgTitle << title << " v " << version;
	setWindowTitle (UTF8TOQSTRING (dlgTitle));
}	// QtAboutDialog::QtAboutDialog


QtAboutDialog::QtAboutDialog (const QtAboutDialog&)
{
	assert (0 && "QtAboutDialog::QtAboutDialog (const QtAboutDialog&) is not allowed.");
}	// QtAboutDialog::QtAboutDialog (const QtAboutDialog&)


QtAboutDialog& QtAboutDialog::operator = (const QtAboutDialog&)
{
	assert (0 && "QtAboutDialog::operator = is not allowed.");
	return *this;
}	// QtAboutDialog::operator = 


QtAboutDialog::~QtAboutDialog ( )
{
}	// QtAboutDialog::~QtAboutDialog


void QtAboutDialog::addAuthor (const UTF8String& author)
{
	_authors.push_back (author);
}	// QtAboutDialog::addAuthor


QtAboutDialog::Component::Component (
		const UTF8String& name, const string& version, const UTF8String& what,
		const string& url)
	: _name (name), _version (version), _what (what), _url (url)
{
}	// Component::Component


QtAboutDialog::Component::Component (const QtAboutDialog::Component& c)
	: _name (c._name), _version (c._version), _what (c._what), _url (c._url)
{
}	// Component::Component


QtAboutDialog::Component& QtAboutDialog::Component::operator = (
											const QtAboutDialog::Component& c)
{
	if (this != &c)
	{
		_name		= c._name;
		_version	= c._version;
		_what		= c._what;
		_url		= c._url;
	}	// if (this != &c)

	return *this;
}	// Component::operator =


void QtAboutDialog::addComponent (
		const UTF8String& name, const string& version, const UTF8String& what,
		const string& url)
{
	_components.push_back (Component (name, version, what, url));
}	// QtAboutDialog::addComponent


void QtAboutDialog::showEvent (QShowEvent* event)
{
	if (0 == _layout)
	{
		QLabel*	label	= 0;
		_layout	= new QVBoxLayout ( );

		UTF8String	title (charset);
		title << _name << " version " << _version;
		label	= new QLabel (UTF8TOQSTRING (title), this);
		QFont	font	= label->font ( );
		font.setBold (true);
		font.setPointSize (2 * font.pointSize ( ));
		label->setFont (font);
		label->setAlignment (Qt::AlignHCenter);
		_layout->addWidget (label);
		label	= new QLabel ("", this);
		label->setAlignment (Qt::AlignHCenter);
		_layout->addWidget (label);
		if (false == _url.empty ( ))
		{
			UTF8String	richText (charset);
			richText << "<A href=" << _url << ">" << _url << "</A>";
			label	= new QLabel (richText.iso ( ).c_str ( ), this);
			label->setOpenExternalLinks (true);
			label->setTextInteractionFlags (Qt::LinksAccessibleByMouse);
			label->setAlignment (Qt::AlignHCenter);
			_layout->addWidget (label);
		}	// if (false == _url.empty ( ))
		if (false == _description.empty ( ))
		{
			label	= new QLabel (QSTR ("Description :"), this);
			QFont	f	 = label->font ( );
			f.setBold (true);
			label->setFont (f);
			_layout->addWidget (label);
			label	= new QLabel (UTF8TOQSTRING (_description), this);
			_layout->addWidget (label);
		}	// if (false == _description.empty ( ))
		if ((false == _description.empty ( )) && 
		    (((0 != _authors.size ( )) || (0 != _components.size ( )))))
		{
			label	= new QLabel ("", this);
			label->setAlignment (Qt::AlignHCenter);
			_layout->addWidget (label);
		}	// if ((false == _description.empty ( )) && ...
		if (0 != _authors.size ( ))
		{
			label	= new QLabel (QSTR ("Réalisation :"), this);
			QFont	f	 = label->font ( );
			f.setBold (true);
			label->setFont (f);
			_layout->addWidget (label);
			for (vector<UTF8String>::const_iterator ita = _authors.begin ( );
			     _authors.end ( ) != ita; ita++)
			{
				label	= new QLabel (UTF8TOQSTRING ((*ita)), this);
				_layout->addWidget (label);
			}	// for (vector<UTF8String>::const_iterator ita = ...
		}	// if (0 != _authors.size ( ))
		if ((0 != _authors.size ( )) && (0 != _components.size ( )))
		{
			label	= new QLabel ("", this);
			label->setAlignment (Qt::AlignHCenter);
			_layout->addWidget (label);
		}	// if ((0 != _authors.size ( )) && (0 != _components.size ( )))
		if (0 != _components.size ( ))
		{
			label	= new QLabel (QSTR ("Composants :"), this);
			QFont	f	 = label->font ( );
			f.setBold (true);
			label->setFont (f);
			_layout->addWidget (label);
			QGridLayout*	componentsLayout	= new QGridLayout ( );
			_layout->addLayout (componentsLayout);
			int	row	= 0;
			label	= new QLabel ("Nom", this);
			label->setAlignment (Qt::AlignHCenter);
			componentsLayout->addWidget (label, row, 0);
			label	= new QLabel ("Version", this);
			label->setAlignment (Qt::AlignHCenter);
			componentsLayout->addWidget (label, row, 1);
			bool	hasDescription	= false, hasUrl	= false;
			row	= 1;
			for (vector<QtAboutDialog::Component>::const_iterator itc =
				_components.begin ( ); _components.end ( ) != itc; itc++, row++)
			{
				label	= new QLabel (UTF8TOQSTRING ((*itc)._name), this);
				componentsLayout->addWidget (label, row, 0);
				label	= new QLabel ((*itc)._version.c_str ( ), this);
				componentsLayout->addWidget (label, row, 1);
				if (false == (*itc)._what.empty ( ))
				{
					label	= new QLabel (UTF8TOQSTRING ((*itc)._what), this);
					componentsLayout->addWidget (label, row, 2);
					hasDescription	= true;
				}	// if (false == (*itc)._what.empty ( ))
				if (false == (*itc)._url.empty ( ))
				{
					UTF8String	richText (charset);
					richText << "<A href=" << (*itc)._url << ">"
					         << (*itc)._url << "</A>";
					label	= new QLabel (richText.iso ( ).c_str ( ), this);
					label->setOpenExternalLinks (true);
					label->setTextInteractionFlags (Qt::LinksAccessibleByMouse);
					componentsLayout->addWidget (label, row, 3);
					hasUrl	= true;
				}	// if (false == (*itc)._url.empty ( ))
			}	// for (vector<Component>::const_iterator itc = ...
			row	= 0;
			if (true == hasDescription)
			{
				label	= new QLabel (QSTR ("Description"), this);
				label->setAlignment (Qt::AlignHCenter);
				componentsLayout->addWidget (label, row, 2);
			}	// if (true == hasDescription)
			if (true == hasUrl)
			{
				label	= new QLabel ("URL", this);
				label->setAlignment (Qt::AlignHCenter);
				componentsLayout->addWidget (label, row, 3);
			}	// if (true == hasUrl)
		}	// if (0 != _components.size ( ))

		QHBoxLayout*	buttonLayout	= new QHBoxLayout ( );
		_layout->addLayout (buttonLayout);
		QPushButton*	closeButton	= new QPushButton ("OK", this);
		closeButton->setFixedSize (closeButton->sizeHint ( ));
		buttonLayout->addStretch (10.);
		buttonLayout->addWidget (closeButton);
		buttonLayout->addStretch (10.);
		connect (closeButton, SIGNAL(clicked ( )), this, SLOT(accept ( )));

		setLayout (_layout);
		_layout->activate ( );
		setFixedSize (sizeHint ( ));
		updateGeometry ( );
	}	// if (0 == _layout)

	QDialog::showEvent (event);
}	// QtAboutDialog::showEvent
