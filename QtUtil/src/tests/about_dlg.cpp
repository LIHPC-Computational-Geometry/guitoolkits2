#include "QtAboutMainWindow.h"
#include "QtUtil/QtHelpWindow.h"
#include "QtUtil/QtValidatedTextField.h"

#include <TkUtil/Exception.h>

#ifndef QT_5
#include <QtGui/QApplication>
#else	// QT_5
#include <QtWidgets/QApplication>
#include <QTextCodec>
#endif	// QT_5


USING_UTIL
USING_STD


static void parseArgs (int argc, char* argv[]);


int main (int argc, char* argv[])
{
	try
	{
//QtValidatedTextField::automaticValidation	= false;
cout << argv [0] << " usage :" << endl
     << argv [0] << "[-helpLocation directory][-helpFile htmlFile]"
     << "[-webPage url][-webTag tag]"
     << endl;
cout << "VERSION DE Qt UTILISEE : "
     << hex << QT_VERSION << " (" << QT_VERSION_STR << ")" << endl;
		parseArgs (argc, argv);

		QApplication		app (argc, argv);
		QtHelpWindow::initialize (
			QtAboutMainWindow::homeURL, QtAboutMainWindow::indexFile,
			"AIDE EN LIGNE", "APP TEST"
				);
		QtAboutMainWindow*	window	= new QtAboutMainWindow (0);
		window->show ( );

		return app.exec ( );
	}
	catch (const Exception& exc)
	{
		cerr << "Erreur : " << exc.getFullMessage ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "Erreur non documentée." << endl;
		return -1;
	}

	return 0;
}	// main


static void parseArgs (int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (string ("-helpLocation") == argv [i])
			QtAboutMainWindow::homeURL		= argv [++i];
		else if (string ("-helpFile") == argv [i])
			QtAboutMainWindow::indexFile	= argv [++i];
		else if (string ("-webPage") == argv [i])
			QtAboutMainWindow::webPage	= argv [++i];
		else if (string ("-webTag") == argv [i])
			QtAboutMainWindow::webTag	= argv [++i];
	}	// for (int i = 0; i < argc; i++)
}	// parseArgs

