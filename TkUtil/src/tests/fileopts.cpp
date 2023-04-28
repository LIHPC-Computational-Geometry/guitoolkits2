#include "TkUtil/LoadableFileSet.h"
#include "TkUtil/Exception.h"

#include <iostream>
#include <stdexcept>


USING_STD
USING_UTIL


int main (int argc, char* argv [])
{
	try
	{
		vector<LoadableFileSet>	files	= 
				LoadableFileSet::createFileSets (argc - 1, argv + 1);

		for (vector<LoadableFileSet>::const_iterator it = files.begin ( );
		     files.end ( ) != it; it++)
		{
		cout << "=============================================================="
		     << endl;
		cout << *it;
		cout << "=============================================================="
		     << endl;
		}	// for (vector<LoadableFileSet>::const_iterator it = ...
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
