#include "PrefsCore/Section.h"
#include "PrefsCore/LongNamedValue.h"
#include "PrefsCore/DoubleNamedValue.h"
#include "PrefsCore/StringNamedValue.h"
#include "PrefsXerces/XMLLoader.h"
#include <TkUtil/Exception.h>

#include <iostream>
#include <stdexcept>
#include <memory>


USING_PREFS
USING_UTIL
USING_STD


int main (int argc, char* argv [])
{
	if (1 == argc)
	{
		cout << "Syntax : " << argv [0] << "xml file" << endl;
		return -1;
	}	// if (1 == argc)

	unique_ptr<Section>	mainSection;

	try
	{
		cout << "Reading configuration in " << argv [1] << " ..." << endl;
		mainSection.reset (XMLLoader::load (argv [1]));
		cout << "Configuration loaded from " << argv [1] << "." << endl;
		cout << *(mainSection.get ( )) << endl;
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


