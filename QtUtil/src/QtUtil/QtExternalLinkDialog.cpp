/**
 * @file		QtExternalLinkDialog.cpp
 * @author		Charles PIGNEROL
 * @date		04/04/2014
 */

#include <QtUtil/QtExternalLinkDialog.h>
#include <QtUtil/QtUnicodeHelper.h>

#include <QUrl>
#include <QDesktopServices>
#ifndef QT_5
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#endif	// QT_5

#include <assert.h>


using namespace TkUtil;
using namespace std;

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtExternalLinkDialog::QtExternalLinkDialog (QWidget* parent,
					const UTF8String& link, const UTF8String& text, const UTF8String& title)
		: QDialog (parent)
{
	setModal (true);
	setWindowTitle (UTF8TOQSTRING (title));

	QVBoxLayout*	vLayout	= new QVBoxLayout (this);
	setLayout (vLayout);

	UTF8String	richText (charset);
	richText << "Cliquez sur le lien suivant "
	         << "<A href=" << link << ">" << text << "</A>"
	         << " pour afficher le document dans le navigateur internet de "
	         << "votre session de travail.";
	QLabel*	label	= new QLabel (UTF8TOQSTRING (richText), this);
	label->setOpenExternalLinks (true);
	label->setTextInteractionFlags (Qt::LinksAccessibleByMouse);
	vLayout->addWidget (label);
	QHBoxLayout*	hlayout	= new QHBoxLayout ( );
	vLayout->addLayout (hlayout);
	hlayout->addStretch (2);
	QPushButton*	closeButton	= new QPushButton ("Fermer", this);
	hlayout->addWidget (closeButton);
	connect (closeButton, SIGNAL(clicked ( )), this, SLOT(accept ( )));
	hlayout->addStretch (2);
}	// QtExternalLinkDialog::QtExternalLinkDialog


QtExternalLinkDialog::QtExternalLinkDialog (const QtExternalLinkDialog&)
	: QDialog (0)
{
	assert (0 && "QtExternalLinkDialog copy constructor is not allowed.");
}	// QtExternalLinkDialog::QtExternalLinkDialog


QtExternalLinkDialog& QtExternalLinkDialog::operator = (
													const QtExternalLinkDialog&)
{
	assert (0 && "QtExternalLinkDialog assignment operator is not allowed.");
	return *this;
}	// QtExternalLinkDialog::QtExternalLinkDialog


QtExternalLinkDialog::~QtExternalLinkDialog ( )
{
}	// QtExternalLinkDialog::~QtExternalLinkDialog


void QtExternalLinkDialog::displayURL (
		QWidget* p, const string& link, const string& text, const string& title)
{
	if (false == QDesktopServices::openUrl (
									QUrl(link.c_str ( ), QUrl::TolerantMode)))
	{
		UTF8String	message (charset);
		message << title << " n'arrive pas à ouvrir l'URL " << link
		        << " avec le navigateur par défaut de la session.";
		QtExternalLinkDialog (p, link, message.iso ( ), title).exec ( );
	}	// if (false == QDesktopServices::openUrl (...
}	// QtExternalLinkDialog::displayURL

