#include "QtUtil/QtWebBrowser.h"
#include "QtUtil/QtAutoWaitingCursor.h"
#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/backward.xpm"
#include "QtUtil/forward.xpm"
#include "QtUtil/home.xpm"
#include "QtUtil/print.xpm"
#include <TkUtil/Exception.h>
#include <TkUtil/File.h>

#include <QIcon>
#include <QPainter>
#include <QUrl>
#include <QFileInfo>
#ifndef QT_5
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#else	// QT_5
#include <QPrinter>
#include <QPrintDialog>
#include <QMainWindow>
#include <QApplication>
#include <QStatusBar>
#include <QToolBar>
#endif	// QT_5
#ifdef USE_QT_WEBKIT
#include <QNetworkCookieJar>
#ifndef QT_5
#include <QtGui/QPrintPreviewDialog>
#else	// QT_5
#include <QPrintPreviewDialog>
#endif	// QT_5
#include <QWebElement>
#include <QWebFrame>
#include <QWebHistory>
#endif	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QWebEngineHistory>
#endif	// USE_QT_WEBENGINE

#include <assert.h>
#include <memory>


USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtWebBrowser::QtWebBrowser (const string& title)
	: QMainWindow (0), _textBrowser (0), _printer (0),
	   _backwardButton (0), _forwardButton (0), _homeButton (0), _printButton (0)
#ifdef USE_QT_WEBKIT
		, _anchorTag ( )
#endif	// USE_QT_WEBKIT
{
	setWindowTitle (title.c_str ( ));

#ifdef USE_QT_WEBKIT
	_textBrowser	= new QWebView (this);
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	_textBrowser	 = new QWebEngineView (this);
#else	// USE_QT_WEBENGINE
	_textBrowser	= new QTextBrowser (this);
	_textBrowser->setFrameStyle (QFrame::Panel | QFrame::Sunken);
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
	setCentralWidget (_textBrowser);
	_textBrowser->setFocus ( );
#ifdef USE_QT_WEBKIT
	connect (_textBrowser, SIGNAL (urlChanged (const QUrl&)),
	         this, SLOT (sourceChanged (const QUrl&)));
	connect (_textBrowser, SIGNAL (loadFinished (bool)), this,
	         SLOT (pageLoaded (bool)));
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	connect (_textBrowser, SIGNAL (urlChanged (const QUrl&)),
	         this, SLOT (sourceChanged (const QUrl&)));
	connect (_textBrowser, SIGNAL (loadFinished (bool)), this,
	         SLOT (pageLoaded (bool)));
#else	// USE_QT_WEBENGINE
	connect (_textBrowser, SIGNAL (sourceChanged (const QUrl&)),
	         this, SLOT (sourceChanged (const QUrl&)));
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT

	createToolBar ( );

	resize (640, 700);
}	// QtWebBrowser::QtWebBrowser


QtWebBrowser::QtWebBrowser (const QtWebBrowser&)
{
	assert (0 && "QtWebBrowser copy constructor is not allowed.");
}	// QtWebBrowser::QtWebBrowser (const QtWebBrowser&)


QtWebBrowser& QtWebBrowser::operator = (const QtWebBrowser&)
{
	assert (0 && "QtWebBrowser operator = is not allowed.");
	return *this;
}	// QtWebBrowser::QtWebBrowser (const QtWebBrowser&)


QtWebBrowser::~QtWebBrowser ( )
{	// Rem : on ne détruit pas _printer, il y a peut être une impression dans
	// une autre tache.
	if (0 != _printer)
	{
		UTF8String	mess (charset);
		mess << "ATTENTION : destruction d'une instance de QtWebBrowser alors "
		     << "qu'une tâche d'impression est en cours.";
		ConsoleOutput::cerr ( ) << mess << co_endl;
	}
	_printer	= 0;
}	// QtWebBrowser::~QtWebBrowser ( )


void QtWebBrowser::print ( )
{
	try
	{

#ifdef USE_QT_WEBENGINE
	_printer	= new QPrinter (QPrinter::ScreenResolution);
#else	// USE_QT_WEBENGINE
	_printer	= new QPrinter (QPrinter::HighResolution);
#endif	// USE_QT_WEBENGINE
	_printer->setFullPage (true);

	unique_ptr<QPrintDialog>	printDialog	(new QPrintDialog (_printer, this));
	printDialog->setWindowTitle ("Impression");	
	if (QDialog::Accepted == printDialog->exec ( ))
	{
		QtAutoWaitingCursor		waitingCursor (true);
#ifdef USE_QT_WEBKIT
	if (true == _textBrowser->url ( ).isValid ( ))
	{
		// On utilise la boite de dialogue  de pré-impression Qt.
		// Pas mal. On regrette par contre qu'elle autorise de modifier les
		// marges (bouton "Page setup", à droite) alors qu'elle y est (au moins
		// dans le cas présent) insensible (dû au fait que c'est du langage
		// HTML, et que les marges sont renseignées par le format (au niveau du
		// css ?) ???)
		QPrintPreviewDialog	previewDlg (_printer);
		connect (&previewDlg, SIGNAL (paintRequested (QPrinter*)), 
				_textBrowser, SLOT (print (QPrinter*)));
		previewDlg.exec ( );
	}	// if (true == _textBrowser->url ( ).isValid ( ))
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	if ((0 != _printer) && (0 != _textBrowser->page ( )))
	{
		if (QPrinter::PdfFormat == _printer->outputFormat ( ))
			_textBrowser->page ( )->printToPdf (
						_printer->outputFileName ( ), _printer->pageLayout ( ));
		else
#ifdef QT_PRINTSUPPORT_LIB
			_textBrowser->page ( )->print (
					_printer, [=](bool){ delete _printer; _printer = 0; });
#else	// QT_PRINTSUPPORT_LIB
		delete _printer;	_printer	= 0;
		throw Exception (UTF8String ("Impression \"papier\" de page web non supportée dans cette version", charset));
#endif	// QT_PRINTSUPPORT_LIB
	}	// if ((0 != prn) && (0 != _textBrowser->page ( )))
#else	// USE_QT_WEBENGINE
	if (0 != _textBrowser->document ( ))
		_textBrowser->document ( )->print (_printer);
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
	else
	{
		delete _printer;	_printer	= 0;
		UTF8String	message (charset);
		message << "Impression impossible : absence de document à imprimer.";
		waitingCursor.hide ( );
		QtMessageBox::displayErrorMessage (
				this, windowTitle ( ).toStdString ( ), message);
		return;
	}	// else if (0 != _textBrowser->document ( ))

		UTF8String	message (charset);
#ifdef USE_QT_WEBKIT
		message << "Document "
		        << _textBrowser->url ( ).toString ( ).toStdString ( )
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
		message << "Document "
		        << _textBrowser->url ( ).toString ( ).toStdString ( )
#else	// USE_QT_WEBENGINE
		message << "Document "
		        << _textBrowser->source ( ).toString ( ).toStdString ( )
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
		        << " imprimé sur " 
		        << (true == _printer->outputFileName ( ).isEmpty ( ) ?
						_printer->printerName ( ).toStdString ( ) :
						_printer->outputFileName ( ).toStdString ( ))
		        << ".";
		waitingCursor.hide ( );
		QtMessageBox::displayInformationMessage (
					this, windowTitle ( ).toStdString ( ), message);
#ifndef USE_QT_WEBENGINE
		delete _printer;	_printer	= 0;
#endif	// USE_QT_WEBENGINE
	}	// if (QDialog::Accepted == printDialog->exec ( ))
	else
	{
		delete _printer;	_printer	= 0;
	}	// else if (QDialog::Accepted == printDialog->exec ( ))

	}
	catch (const Exception& exc)
	{
		delete _printer;	_printer	= 0;
		UTF8String	message (charset);
		message << "Echec de l'impression :\n" << exc.getFullMessage ( );
		QtMessageBox::displayErrorMessage (
				this, windowTitle ( ).toStdString ( ), message);
	}
	catch (const exception& e)
	{
		delete _printer;	_printer	= 0;
		UTF8String	message (charset);
		message << "Echec de l'impression :\n" << e.what ( );
		QtMessageBox::displayErrorMessage (
				this, windowTitle ( ).toStdString ( ), message);
	}
	catch (...)
	{
		delete _printer;	_printer	= 0;
		UTF8String	message (
				"Echec de l'impression : erreur non renseignée.", charset);
		QtMessageBox::displayErrorMessage (
				this, windowTitle ( ).toStdString ( ), message);
	}
}	// QtWebBrowser::print


void QtWebBrowser::sourceChanged (const QUrl& source)
{
	assert ((0 != statusBar ( )) && "QtWebBrowser::sourceChanged : null status bar.");
	statusBar ( )->showMessage (source.toString ( ));

#ifdef USE_QT_WEBKIT
	assert (0 != _backwardButton);
	assert (0 != _forwardButton);
	assert (0 != _textBrowser);
	if (0 != _textBrowser->history ( ))
	{
		_backwardButton->setEnabled (_textBrowser->history ( )->canGoBack ( ));
		_forwardButton->setEnabled (_textBrowser->history ( )->canGoForward ( ));
	}	// if (0 != _textBrowser->history ( ))
#else
#ifdef USE_QT_WEBENGINE
	assert (0 != _backwardButton);
	assert (0 != _forwardButton);
	assert (0 != _textBrowser);
	if (0 != _textBrowser->history ( ))
	{
		_backwardButton->setEnabled (_textBrowser->history ( )->canGoBack ( ));
		_forwardButton->setEnabled (_textBrowser->history ( )->canGoForward ( ));
	}	// if (0 != _textBrowser->history ( ))
#else	// USE_QT_WEBENGINE
	try
	{
		File	f (source.toString ( ).toStdString ( ));
		QStringList	paths;
		paths.append (f.getPath ( ).getFullFileName ( ).c_str ( ));
		_textBrowser->setSearchPaths (paths);
	}
	catch (...)
	{
	}
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
}	// QtWebBrowser::sourceChanged



void QtWebBrowser::homeCallback ( )
{
	assert ((0 != _textBrowser) && "QtWebBrowser::homeCallback : null text browser.");
#ifdef USE_QT_WEBKIT
	QWebHistory*	history	= _textBrowser->history ( );
	if ((0 != history) && (0 != history->count ( )))
		history->goToItem (history->itemAt (0));
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	QWebEngineHistory*	history	= _textBrowser->history ( );
	if ((0 != history) && (0 != history->count ( )))
		history->goToItem (history->itemAt (0));
#else	// USE_QT_WEBENGINE
	ConsoleOutput::cerr ( )
	     << "QtWebBrowser::homeCallback non disponible pour les versions de Qt "
	     << "comprises entre les versions 4.7.0 et 5.8.x." << co_endl;
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
}	// QtWebBrowser::homeCallback


void QtWebBrowser::pageLoaded (bool ok)
{
#ifdef USE_QT_WEBKIT
	if ((0 != _anchorTag.length ( )) && (0 != _textBrowser) &&
	    (0 != _textBrowser->page ( )) &&
	    (0 != _textBrowser->page ( )->mainFrame ( )))
		_textBrowser->page ( )->mainFrame ( )->scrollToAnchor (
														_anchorTag.c_str ( ));
// On n'efface pas volontairement le tag. Ca ne sert à rien et peut même poser
// des problèmes car le chargement de la page se fait dans un autre thread.
// => En cas de concurrence, la première page chargée efface le tag d'une
// éventuelle requête ultérieure => l'effacement est fait dans le thread
// principal au niveau de setURL.
//	_anchorTag.clear ( );
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	assert ((0 != _textBrowser) && "QtWebBrowser::pageLoaded : null text browser.");
	assert ((0 != _textBrowser->history ( )) && "QtWebBrowser::pageLoaded : null web browser history.");
	assert ((0 != _backwardButton) && "QtWebBrowser::pageLoaded : no backward button.");
	assert ((0 != _forwardButton) && "QtWebBrowser::pageLoaded : no forward button.");
	_backwardButton->setEnabled (_textBrowser->history ( )->canGoBack ( ));
	_forwardButton->setEnabled (_textBrowser->history ( )->canGoForward ( ));
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
}	// QtWebBrowser::pageLoaded


void QtWebBrowser::setURL (const string& url, const string& tag)
{	
	assert ((0 != _textBrowser) && "QtWebBrowser::setURL : null text browser.");
	string	source	= url;
/*#ifndef USE_QT_WEBKIT
	// Pour Qt 3 et 4 si < 4.0.7 : la balise est ajoutée à l'URL
	if (0 != tag.length ( ))
	{
		source	+= "#";
		source	+= tag;
	}	// if (0 != tag.length ( ))
#endif	// USE_QT_WEBKIT*/

#ifdef USE_QT_WEBKIT
	_anchorTag	= tag;
	_textBrowser->load (QUrl (source.c_str ( )));
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	// Il est dans ce cas présent indispensable de préciser ici le protocole
	// utilisé dans l'URL, et ce comme suit : protocole:url
	_anchorTag	= tag;
	UTF8String	loadedUrl;
	string::size_type	pos	= source.find (':');
	if (string::npos == pos)
		loadedUrl << "file://";
	loadedUrl << source;
	if (false == tag.empty ( ))
		loadedUrl << '#' << tag;
	_textBrowser->setUrl (QUrl (loadedUrl.ascii ( ).c_str ( )));
#else	// USE_QT_WEBENGINE
	_textBrowser->setSource (QUrl (source.c_str ( )));
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
}	// QtWebBrowser::setURL


void QtWebBrowser::createToolBar ( )
{
	QToolBar*	toolBar	= new QToolBar ("Navigation", this);
	addToolBar (Qt::TopToolBarArea, toolBar);

	QPixmap			backwardPixmap (backwardIcon);
	QPixmap			forwardPixmap (forwardIcon);
	QPixmap			homePixmap (homeIcon);
	QPixmap			printPixmap (printIcon);
	const QIcon	iconBackward (backwardPixmap);
	const QIcon	iconForward (forwardPixmap);
	const QIcon	iconHome (homePixmap);
	const QIcon	iconPrint (printPixmap);

	_backwardButton	= new QToolButton (toolBar);
	_backwardButton->setIcon (iconBackward);
	toolBar->addWidget (_backwardButton);
	connect (_backwardButton, SIGNAL (clicked ( )), _textBrowser,
#ifdef USE_QT_WEBKIT
	         SLOT (back ( )));
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	         SLOT (back ( )));
#else	// USE_QT_WEBENGINE
	         SLOT (backward ( )));
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT
	_forwardButton	= new QToolButton (toolBar);
	_forwardButton->setIcon (iconForward);
	toolBar->addWidget (_forwardButton);
	connect (_forwardButton, SIGNAL (clicked ( )), _textBrowser,
	         SLOT (forward ( )));

	toolBar->addSeparator ( );

	_homeButton	= new QToolButton (toolBar);
	_homeButton->setIcon (iconHome);
	toolBar->addWidget (_homeButton);
#ifdef USE_QT_WEBKIT 
	connect (_homeButton, SIGNAL (clicked ( )), this, SLOT (homeCallback ( )));
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	connect (_homeButton, SIGNAL (clicked ( )), this, SLOT (homeCallback ( )));
#else	// USE_QT_WEBENGINE
	connect (_homeButton, SIGNAL (clicked ( )), _textBrowser, SLOT (home ( )));
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT

	toolBar->addSeparator ( );

	_printButton	= new QToolButton (toolBar);
	_printButton->setIcon (iconPrint);
	toolBar->addWidget (_printButton);
	connect (_printButton, SIGNAL (clicked ( )), this, SLOT (print ( )));
	assert (0 != _backwardButton);
	assert (0 != _forwardButton);
#ifndef USE_QT_WEBKIT
#ifndef USE_QT_WEBENGINE
	connect (_textBrowser, SIGNAL (backwardAvailable (bool)), _backwardButton,
	         SLOT (setEnabled (bool)));
	connect (_textBrowser, SIGNAL (forwardAvailable (bool)), _forwardButton,
	         SLOT (setEnabled (bool)));
#endif	// USE_QT_WEBENGINE
	_backwardButton->setEnabled (false);
	_forwardButton->setEnabled (false);
#endif	// USE_QT_WEBKIT
}	// QtWebBrowser::createToolBar

