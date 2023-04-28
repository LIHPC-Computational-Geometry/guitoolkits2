#include <PrefsQt/QtPrefsUpdater.h>
#include <QtUtil/QtMessageBox.h>
#include <QtUtil/QtUnicodeHelper.h>
#include <TkUtil/Exception.h>

#ifndef QT_5
#include <QtGui/QApplication>
#else	// QT_5
#include <QApplication>
#endif	// QT_5

#include <iostream>
#include <stdexcept>

USING_STD
USING_UTIL
USING_PREFS

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


int main (int argc, char* argv [])
{
	bool				confirm	= true;
	string				config, patchs;
	size_t				count	= 0;
	const string		appTitle (argv [0]);
	const UTF8String	context (
		"L'application effectue une mise à jour des préférences utilisateur.",
		charset);

	ConsoleOutput::cout ( ) << "Syntaxe : " << argv [0]
	     << " -config preferences.xml -updates correctifs.xml [-force]"
	     << co_endl << co_endl;

	for (int i = 1; i < argc; i++)
		if (string ("-force") == argv [i])
			confirm	= false;
		else if (i < argc - 1)
		{
			if (string ("-config") == argv [i])
				config	= argv [++i];
			else if (string ("-updates") == argv [i])
				patchs	= argv [++i];
		}	// else if (i < argc - 1)

	QApplication	application (argc, argv);

	try
	{
		UTF8String	modifications (charset);
		count	= QtPrefsUpdater::update (
					config, patchs, appTitle, context, confirm, modifications);
		if (0 == count)
			modifications << "Absence de modifications.";
		QtMessageBox::displayInformationMessage (0, argv [0], modifications);
	}
	catch (const Exception& exc)
	{
		QtMessageBox::displayErrorMessage (0, argv [0], exc.getFullMessage ( ));
		ConsoleOutput::cout ( ) << "Exception caught : "
			<< exc.getMessage ( ) << co_endl;
		return -1;
	}
	catch (const exception& stdExc)
	{
		QtMessageBox::displayErrorMessage (0, argv [0], stdExc.what ( ));
		ConsoleOutput::cout ( ) << "Standard exception caught : "
			<< stdExc.what ( ) << co_endl;
		return -1;
	}
	catch (...)
	{
		QtMessageBox::displayErrorMessage (0, argv [0], "Erreur non documentée");
		ConsoleOutput::cout ( ) << "Unexpected exception caught." << co_endl;
		return -1;
	}

	return 0;
}	// main
