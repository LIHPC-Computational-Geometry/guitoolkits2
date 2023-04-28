#include "PrefsQt/QtPreferencesDialog.h"
#include "PrefsXerces/XMLLoader.h"
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


int main (int argc, char* argv [])
{
	bool	editable	= true;
	string	fileName;

	for (int i = 1; i < argc; i++)
		if (string ("-read") == argv [i])
			editable	= false;
		else if ('-' != argv [i][0])
			fileName	= argv [i];

	try
	{
		QApplication	application (argc, argv);
		Section*		section	= 0;
		if (0 != fileName.length ( ))
			section		= XMLLoader::load (fileName);
		QtPreferencesDialog*	dialog	=
			new QtPreferencesDialog (0, section, fileName, editable, false, 
			                         true, "Config Editor", "Config Editor");
		dialog->exec ( );
		delete dialog;
	}
	catch (const Exception& exc)
	{
		ConsoleOutput::cout ( ) << "Exception caught : " << exc.getMessage ( )
			<< co_endl;
		return -1;
	}
	catch (const exception& stdExc)
	{
		ConsoleOutput::cout ( ) << "Standard exception caught : "
			<< stdExc.what ( ) << co_endl;
		return -1;
	}
	catch (...)
	{
		ConsoleOutput::cout ( ) << "Unexpected exception caught." << co_endl;
		return -1;
	}

	return 0;
}	// main
