#include "QtUtil/QtHelpWindow.h"
#include "QtUtil/QtAutoWaitingCursor.h"
#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/backward.xpm"
#include "QtUtil/forward.xpm"
#include "QtUtil/home.xpm"
#include "QtUtil/print.xpm"

#include <assert.h>
#include <stdlib.h>	// putenv
#include <memory>


USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION

QtHelpWindow*	QtHelpWindow::_instance		= 0;
string			QtHelpWindow::_helpRootURL;
string			QtHelpWindow::_helpIndexURL;
string			QtHelpWindow::_appTitle;
string			QtHelpWindow::_windowTitle;



QtHelpWindow::QtHelpWindow (const string& title)
	: QtWebBrowser (title)
{
}	// QtHelpWindow::QtHelpWindow


QtHelpWindow::QtHelpWindow (const QtHelpWindow&)
	: QtWebBrowser ("Invalid window")
{
	assert (0 && "QtHelpWindow copy constructor is not allowed.");
}	// QtHelpWindow::QtHelpWindow (const QtHelpWindow&)

QtHelpWindow& QtHelpWindow::operator = (const QtHelpWindow&)
{
	assert (0 && "QtHelpWindow operator = is not allowed.");
	return *this;
}	// QtHelpWindow::QtHelpWindow (const QtHelpWindow&)


QtHelpWindow::~QtHelpWindow ( )
{
	_instance	= 0;
}	// QtHelpWindow::~QtHelpWindow ( )


void QtHelpWindow::initialize (const string& homeURL, const string& indexURL,
                               const string& title, const string& appTitle)
{
	if ((0 != homeURL.size ( )) && ('/' != homeURL [homeURL.size ( ) - 1]))
		_helpRootURL	= homeURL + '/';
	else
		_helpRootURL	= homeURL;
	_helpIndexURL	= indexURL;
	_appTitle		= appTitle;
	_windowTitle	= title;
#ifdef USE_QT_WEBENGINE
// Eviter d'avoir le message :
// No usable sandbox! Update your kernel or see https://chromium.googlesource.com/chromium/src/+/master/docs/linux_suid_sandbox_development.md ...
// dans le terminale de lancement.
// L'idéal étant de fournir --no-sandbox comme argument de ligne de commande
// à la QApplication.
//	putenv ("QTWEBENGINE_DISABLE_SANDBOX=1");
//	A la réflexion on désactive également l'utilisation de GL qui fait mauvais
//	ménage avec VTK : (merci Guéno)
	putenv ("QTWEBENGINE_CHROMIUM_FLAGS=\"--no-sandbox --disable-gpu --disable-logging --single-process\"");
#endif	// USE_QT_WEBENGINE
}	// QtHelpWindow::initialize


void QtHelpWindow::finalize ( )
{
	delete _instance;
	_instance		= 0;
	_helpRootURL	= "";
	_appTitle		= "";
	_windowTitle	= "";
}	// QtHelpWindow::finalize


void QtHelpWindow::setURL (const string& url, const string& tag)
{
	UTF8String	fullURL (charset);
//	fullURL << _helpRootURL << "/" << url;
	fullURL << _helpRootURL << url;
	QtWebBrowser::setURL (fullURL, tag);
}	// QtHelpWindow::setURL


void QtHelpWindow::displayURL (const string& url)
{
	getInstance ( ).setURL (url, "");
	getInstance ( ).show ( );
	getInstance ( ).raise ( );
}	// QtHelpWindow::displayURL


void QtHelpWindow::displayURL (const string& url, const string& tag)
{
// Patch CP : Le mécanisme normal ne marche pas toujours en Qt 4.8.5,
// notamment avec M3D : si les images sont accessibles le positionnement sur la
// balise demandée marche au premier appel, puis ne marche plus. Si les images
// ne sont pas accessibles ça marche en toutes circonstances ...
// On contourne le bogue ici en prenant l'ancien mécanisme, qui présente
// l'inconvénient d'être purement HTML : fichier#balise
#if QT_VERSION >= 0x040800	// Ne marche pas en Qt 4.7.4 :
	UTF8String	u (charset);
	u << url;
	if (false == tag.empty ( ))
		u << "#" << tag;
	getInstance ( ).setURL (u.iso ( ), "");	// Contournement bogue Qt 4.8.5
#else	// Ne marche pas en Qt 4.8.5 :
	getInstance ( ).setURL (url, tag);	// Bogue Qt 4.8.5 si images
#endif

	getInstance ( ).show ( );
	getInstance ( ).raise ( );
}	// QtHelpWindow::displayURL


QtHelpWindow& QtHelpWindow::getInstance ( )
{
	if (0 == _instance)
	{
		_instance	= new QtHelpWindow (_windowTitle);
		assert ((0 != _instance) && "QtHelpWindow::getInstance : null help window.");
		// La fonction "home" de QTextBrowser renvoi au premier fichier
		// charge. On s'assure ici qu'il s'agira bien de l'index.
		UTF8String	indexURL (charset);
#ifdef QT_4
		indexURL << _helpRootURL << "/" << _helpIndexURL;
#else	// QT_4
		indexURL << _helpIndexURL;
#endif	// QT_4
		_instance->setURL (indexURL, "");
	}	// if (0 == _instance)

	return *_instance;
}	// QtHelpWindow::getInstance


