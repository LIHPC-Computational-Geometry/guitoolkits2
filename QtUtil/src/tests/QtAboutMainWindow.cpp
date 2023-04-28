#include <QtAboutMainWindow.h>
#include <QtUtil/QtAboutDialog.h>
#include <QtUtil/QtDlgClosurePanel.h>
#include <QtUtil/QtExternalLinkDialog.h>
#include <QtUtil/QtHelpWindow.h>
#include <QtUtil/QtCoordinatesDialog.h>
#include <QtUtil/QtMessageBox.h>
#include <QtUtil/QtMatrixDialog.h>
#include <QtUtil/QtMinMaxDialog.h>
#include <QtUtil/QtRotationDialog.h>
#include <QtUtil/QtScientificTextDialog.h>
#include <QtUtil/QtStringHelper.h>
#include <QtUtil/QtStringListSelectionPanel.h>
#include <QtUtil/QtUtilInfos.h>
#include <QtUtil/QtUnicodeHelper.h>
#include <TkUtil/ErrorLog.h>
#include <TkUtil/InformationLog.h>
#include <TkUtil/NumericServices.h>
#include <TkUtil/OstreamLogOutputStream.h>
#include <TkUtil/TraceLog.h>
#include <TkUtil/WarningLog.h>
#include <TkUtil/UTF8String.h>
#include <TkUtil/UtilInfos.h>

#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#else	// QT_5
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#endif	// QT_5
#include "QtUtil/CharToLatinQString.h"


USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");
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
//                          LA CLASSE QtAboutMainWindow
// ===========================================================================


string  QtAboutMainWindow::homeURL ("/usr/share/cups/doc-root");
string	QtAboutMainWindow::indexFile ("index.html");
string	QtAboutMainWindow::webPage ("https://vtk.org/doc/nightly/html");
string	QtAboutMainWindow::webTag ("");


QtAboutMainWindow::QtAboutMainWindow (QWidget* parent)
	: QMainWindow (parent), _webBrowser (0), _logView (0), _statusLogOutputStream (0), _logDispatcher ( )
{
	QMenu*	menu	= new QMenu ("Application", this);
	menuBar ( )->addMenu (menu);
	menu->setToolTipsVisible (true);
	menu->addAction ("Navigateur web ...", this, SLOT (webCallback ( )));
	menu->addAction ("Aide ...", this, SLOT (helpCallback ( )));
	menu->addAction ("Navigateur web de la session de travail ...", this, SLOT (sessionWebBrowserCallback ( )));
	menu->addAction (QString("Coordonnées ..."), this, SLOT (coordinatesCallback ( )));
	menu->addAction ("Rotation ...", this, SLOT (rotationCallback ( )));
	menu->addAction ("Matrice de transformation ...", this, SLOT (transformationMatrixCallback ( )));
	menu->addAction ("Min / Max ...", this, SLOT (minMaxCallback ( )));
	menu->addAction ("Texte scientifique ...", this, SLOT (scientificTextCallback ( )));
	menu->addAction ("Avertissement ...", this, SLOT (warningCallback ( )));
	menu->addAction ("Erreur ...", this, SLOT (errorCallback ( )));
	menu->addAction ("Listes de chaînes de caractères ...", this, SLOT (stringListCallback ( )));
	menu->addAction ("A propos de ...", this, SLOT (aboutCallback ( )));
	menu->addAction ("Quitter", this, SLOT (exitCallback ( )));

	_logView	= new QtLogsView (this, Log::ALL_KINDS);
	setCentralWidget (_logView);
	_logDispatcher.addStream (_logView);
	_statusLogOutputStream	= new QtStatusLogOutputStream (statusBar ( ), Log::ALL_KINDS);
	_logDispatcher.addStream (_statusLogOutputStream);
	OstreamLogOutputStream*	os	= new OstreamLogOutputStream (cout, Log::ALL_KINDS);
	_logDispatcher.addStream (os);
	TRACE_LOG_3 (l, "QtAboutMainWindow::QtAboutMainWindow : IHM créée.")
	log (l);
}	// QtAboutMainWindow::QtAboutMainWindow


void QtAboutMainWindow::log (const Log& l)
{
	_logDispatcher.log (l);
}	// QtAboutMainWindow::log


void QtAboutMainWindow::webCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	log (WarningLog (UTF8String ("Navigateur web appelé.", charset)));
	if (0 == _webBrowser)
	{
		_webBrowser	= new QtWebBrowser (windowTitle ( ).toStdString ( ));
		_webBrowser->setURL (webPage, webTag);
	}	// if (0 == _webBrowser)
	_webBrowser->show ( );
	_webBrowser->raise ( );

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::webCallback

void QtAboutMainWindow::helpCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	log (WarningLog (UTF8String ("Aide appelée.", charset)));
	QtHelpWindow::displayURL (indexFile, "");
/*string	url	= homeURL + "/" + indexFile;
cout << "URL=" << url<< endl;
	QtHelpWindow::displayURL (url, "");*/

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::helpCallback


void QtAboutMainWindow::sessionWebBrowserCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	log (WarningLog (UTF8String ("Navigateur web de la session de travail appelé.", charset)));
	QtExternalLinkDialog (this, webPage, UTF8String ("Wiki (de qualité)", charset), UTF8String ("Être aidé", charset)).exec ( );

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::sessionWebBrowserCallback


void QtAboutMainWindow::coordinatesCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	UTF8String	message (charset);
	message << "Choix des coordonnées.";
	static double	x = 0., y = 0., z = 0.;
	QtCoordinatesDialog	dialog (this, "APP : Choix des coordonnées", "X", "Y", "Z", x, y, z, true, true, true, true);
	if (QDialog::Accepted != dialog.exec ( ))
		return;

	message << "Avant : (" << x << ", " << y << ", " << z << ")" << "\n";
	bool	mx = false, my = false, mz = false;
	double	nx = 0., ny = 0., nz = 0.;
	try
	{
		dialog.getActivations (mx, my, mz);
		dialog.getCoordinates (nx, ny, nz);
	}
	catch (...)
	{
	}
	if (true == mx)	x	= nx;
	if (true == my)	y	= ny;
	if (true == mz)	z	= nz;
	message << "Après : (" << x << ", " << y << ", " << z << ")";
	log (InformationLog (message));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::coordinatesCallback


void QtAboutMainWindow::rotationCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	UTF8String				message (charset);
	static unsigned char	axe = 0;
	static double			angle = 0.;
	QtRotationDialog		dialog (this, "APP : Rotation", UTF8STR ("Abscisses"), UTF8STR ("Ordonnées"), UTF8STR ("Elévation"),
			axe, angle, UTF8STR ("degrés"), -90., 90.);
	if (QDialog::Accepted != dialog.exec ( ))
		return;
	axe		= dialog.getAxe ( );
	angle	= dialog.getAngle ( );

	message << "Rotation de " << angle << " degrés de l'axe ";	
	switch (axe)
	{
		case 0	: message << "des abscisses.";	break;
		case 1	: message << "des ordonnées.";	break;
		default : message << "des élévations.";
	}	// switch (axe)
	log (InformationLog (message));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::rotationCallback


void QtAboutMainWindow::transformationMatrixCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	QtMatrixDialog	dialog (this, "APP : Matrice de transformation", 3, 3);
	if (QDialog::Accepted != dialog.exec ( ))
		return;

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::transformationMatrixCallback


void QtAboutMainWindow::minMaxCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	UTF8String		message (charset);
	static double	min	= 0.,	max	= 0.;

	QtDoubleMinMaxDialog	dialog (this, "APP", "Saisie des angles extrema", "Minimum", "Maximum", min, max, true, "classQtMinMaxDialog.html", "_details");
	dialog.setMinRange (0., 360.);
//dialog.setMinRange (-NumericServices::doubleMachMax, NumericServices::doubleMachMax);
	dialog.setMaxRange (0., 360.);
//	dialog.setMaxRange (0., 2.3-05);
	if (QDialog::Accepted != dialog.exec ( ))
		return;

	min	= dialog.getMin ( );
	max	= dialog.getMax ( );
	message << "Saisie des extrema. Min = " << min << ", Max = " << max << ".";
	log (InformationLog (message));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::minMaxCallback


void QtAboutMainWindow::scientificTextCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	static	QString	text	= QString ("Exemple de texte scientifique : ") +
						 	  QtStringHelper::roMin ( ) + " = f (" +
							  QtStringHelper::thetaMaj ( ) + ")";
	QtScientificTextDialog	dialog (
			this, "APP", QtUnicodeHelper::qstringToUTF8String (text), true);
	if (QDialog::Accepted != dialog.exec ( ))
		return;

	text	= dialog.getQText ( );
	log (InformationLog (QtUnicodeHelper::qstringToUTF8String (text)));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::scientificTextCallback


void QtAboutMainWindow::warningCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	QtMessageBox::displayWarningMessage (this, "AVERTISSEMENT", "Message d'avertissement");
	log (WarningLog (UTF8String ("Message d'avertissement", charset)));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::warningCallback


void QtAboutMainWindow::errorCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	QtMessageBox::displayErrorMessage (this, "ERREUR", "Message d'erreur");
	log (ErrorLog (UTF8String ("Message d'erreur", charset)));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::errorCallback


void QtAboutMainWindow::stringListCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	vector<string>		unselected, selected;
	for (size_t i = 0; i < 10; i++)
	{
		UTF8String	word;
		word << "Mot " << i;
		unselected.push_back (word);
	}
	QStringListDialog	dialog (this, unselected, selected);
	
	if (QDialog::Accepted != dialog.exec ( ))
		return;
	selected	= dialog.getSelection ( );

	UTF8String	message (charset);
	message << "Sélection : ";
	for (vector<string>::const_iterator it = selected.begin ( );
	     selected.end ( ) != it; it++)
	{
		if (selected.begin ( ) != it)
			message << ", ";
		message << *it;
	}
	message << ".";
	log (InformationLog (message));

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::stringListCallback


void QtAboutMainWindow::aboutCallback ( )
{
	BEGIN_TRY_CATCH_BLOCK

	const char*	description ("L'application en 3 lignes :\n- Test de la classe QtAboutDialog\n- Test de la description\n- Test de l'aspect multilignes\n- Test de l'auto-détection de l'encodage des fichiers sources.");
	QtAboutDialog	dialog (this, "App. test", "1.0.0", "http://www.myapp.com", UTF8STR (description));
	dialog.addAuthor ("Charles PIGNEROL CEA/DAM/DSSI (5291)");
	dialog.addComponent ("QtUtil", QtUtilInfos::getDetailedVersion ( ), "Utilitaires pour Qt", "doc/QtUtil/index.html");
	dialog.addComponent ("TkUtil", UtilInfos::getVersion ( ).getVersion ( ), "Utilitaires pour Qt", "doc/QtUtil/index.html");
	dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK
}	// QtAboutMainWindow::aboutCallback


void QtAboutMainWindow::exitCallback ( )
{
	switch (QtMessageBox::displayQuestionMessage (this, "QUITTER", UTF8STR ("Souhaitez-vous réellement quitter l'application ?"), 100, "OK", "Annuler", 0, 0))
	{
		case	0	:
			delete _webBrowser;	_webBrowser	= 0;	// Sinon plante en Qt 5.9.1
			QtHelpWindow::finalize ( );				// Sinon plante en Qt 5.9.1
			QtStringHelper::finalize ( );
			QApplication::exit (0);
			break;
	}
}	// QtAboutMainWindow::exitCallback



// ===========================================================================
//                          LA CLASSE QStringListDialog
// ===========================================================================

QStringListDialog::QStringListDialog (QWidget* parent, const vector <string>& unselected, const vector <string>& selected)
	: QDialog (parent, QtConfiguration::modalDialogWFlags), _stringListPanel (0)
{
	setModal (true);
	setWindowTitle ("APP");

	QVBoxLayout*	layout	= new QVBoxLayout (this);
	layout->setMargin (QtConfiguration::margin);
	layout->setSizeConstraint (QLayout::SetMinimumSize);

	_stringListPanel	=	 new QtStringListSelectionPanel (this, unselected, selected);
	layout->addWidget (_stringListPanel);

	QtDlgClosurePanel*	closurePanel	= new QtDlgClosurePanel (this, false, "Appliquer", "", "Annuler");
	layout->addWidget (closurePanel);
	closurePanel->setMinimumSize (closurePanel->sizeHint ( ));
	connect (closurePanel->getApplyButton ( ), SIGNAL(clicked ( )), this, SLOT(accept ( )));
	connect (closurePanel->getCancelButton ( ), SIGNAL(clicked ( )), this, SLOT(reject ( )));
}	// QStringListDialog::QStringListDialog


QStringListDialog::QStringListDialog (const QStringListDialog&)
{
	assert (0 && "QStringListDialog copy constructor is not allowed.");
}	// QStringListDialog::QStringListDialog (const QStringListDialog&)


QStringListDialog& QStringListDialog::operator = (const QStringListDialog&)
{
	assert (0 && "QStringListDialog operator = is not allowed.");
	return *this;
}	// QStringListDialog::operator =


QStringListDialog::~QStringListDialog ( )
{
}	// QStringListDialog::~QStringListDialog


vector <string> QStringListDialog::getSelection ( ) const
{
	assert (0 != _stringListPanel);
	return _stringListPanel->getSelection ( );
}	// QStringListDialog::getSelection


