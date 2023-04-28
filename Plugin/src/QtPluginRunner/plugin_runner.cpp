#include "QtPlugin/QtPluginRunnerDialog.h"
#include <PrefsXerces/XMLLoader.h>
#include <PrefsCore/StringNamedValue.h>
#include <TkUtil/Exception.h>

#include <QApplication>
#include <iostream>
#include <stdexcept>
#include <assert.h>

USING_STD
USING_UTIL
USING_PREFS
using namespace PLUGIN;


// Implementation de core.h
void writeMessage (const string& message)
{
	cout << message << endl;
}	// writeMessage


int main (int argc, char* argv [])
{
	string	fileName;

	for (int i = 1; i < argc - 1; i++)
		if (string ("-plugin") == argv [i])
			fileName	= argv [i + 1];

	try
	{
		QApplication	application (argc, argv);
		Section*		section	= 0;
		if (0 != fileName.length ( ))
			section		= XMLLoader::load (fileName);

		assert (0 != section);
		StringNamedValue&	libValue	= 
			dynamic_cast<StringNamedValue&>(section->getNamedValue ("library"));
		const string	libName	= libValue.getStrValue ( );
		StringNamedValue&	functionValue	= 
			dynamic_cast<StringNamedValue&>(section->getNamedValue("function"));
		const string	functionName	= functionValue.getStrValue ( );

		Plugin	plugin (libName, functionName);
		QtPluginRunnerDialog*	dialog	=
			new QtPluginRunnerDialog (0, section, &plugin, fileName, 
			                          false, true,
			                          "Plugin runner", "Plugin runner");
		dialog->exec ( );
		delete dialog;
	}
	catch (const Exception& exc)
	{
		cout << "Exception caught : " << exc.getMessage ( ) << endl;
		return -1;
	}
	catch (const exception& stdExc)
	{
		cout << "Standard exception caught : " << stdExc.what ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cout << "Unexpected exception caught." << endl;
		return -1;
	}

	return 0;
}	// main
