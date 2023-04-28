#include "TkUtil/Exception.h"
#include "TkUtil/NumericConversions.h"

#include <iostream>
#include <string>

using namespace std;
using namespace TkUtil;


static bool parseArgs (int argc, char* argv []);
static int syntax (const string& pgm);

static double			number			= 0.;
static unsigned short	precision		= 6;
static unsigned short	integerPartNum	= 1;


int main (int argc, char* argv [])
{
	if (false == parseArgs (argc, argv))
		return syntax (argv [0]);

	cout << "NumericConversions::userRepresentation (" << argv [1]
	     << ", " << (unsigned long)precision 
	     << ", " << (unsigned long)integerPartNum << ") = " 
	     << NumericConversions::userRepresentation (
									number, precision, integerPartNum) << endl
	     << "NumericConversions::shortestRepresentation (" << argv [1]
	     << ", " << (unsigned long)precision 
	     << ", " << (unsigned long)integerPartNum << ") = " 
	     << NumericConversions::shortestRepresentation (
									number, precision, integerPartNum) << endl;

	return 0;
}	// main


static bool parseArgs (int argc, char* argv [])
{
	if ((1 >= argc) || (4 < argc))
		return false;

	try
	{
		number	= NumericConversions::strToDouble (argv [1]);
		if (3 <= argc)
			precision	= NumericConversions::strToULong (argv [2]);
		if (4 <= argc)
			integerPartNum	= NumericConversions::strToULong (argv [3]);
	}
	catch (const Exception& e)
	{
		cerr << e.getFullMessage ( ) << endl;
		return false;
	}
	catch (...)
	{
		cerr << "Erreur non documentée." << endl;
		return false;
	}

	return true;
}	// parseArgs


static int syntax (const string& pgm)
{
	cout << pgm << " nombre [précision] [taille_partie_entière]" << endl;

	return -1;
}	// syntax



