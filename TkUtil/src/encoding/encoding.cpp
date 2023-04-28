//
// Programme test de l'auto-détection de l'encodage des sources.
//
#include "Iso8859_1.h"
#include "UTF_8.h"
#include "TkUtil/Exception.h"
#include <iostream>

using namespace TkUtil;
using namespace std;


int main (int argc, char* argv[])
{
	try
	{
		cout << iso8859_1Str ( ) << endl;
		cout << utf8Str ( ) << endl;
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

