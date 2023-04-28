#include "TkUtil/UserData.h"
#include "TkUtil/Exception.h"

#include <iostream>
#include <stdexcept>


USING_STD
USING_UTIL


int main (int argc, char* argv [])
{
	try
	{
		UserData	userData;
		if (1 < argc)
			userData	= UserData (string (argv [1]));

		cout << "=============================================================="
		     << endl;
		cout << userData;
		cout << "=============================================================="
		     << endl;
	}
	catch (const Exception& exc)
	{
		cout << "Exception caught : " << exc.getFullMessage ( ) << endl;
		return -1;
	}
	catch (const exception& e)
	{
		cout << "Standard exception caught : " << e.what ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cout << "Undocumented exception caught." << endl;
		return -1;
	}

	return 0;
}	// main
