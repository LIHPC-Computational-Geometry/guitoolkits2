//
// Programme test de l'auto-détection de l'encodage des sources pour
// conversion correcte en QString des chaînes de caractères contenues.
//
#include "QtEncodingMainWindow.h"

#include <TkUtil/Exception.h>

#include <QApplication>


USING_UTIL
USING_STD


int main (int argc, char* argv[])
{
	try
	{
cout << "VERSION DE Qt UTILISEE : "
     << hex << QT_VERSION << " (" << QT_VERSION_STR << ")" << endl;

		QApplication			app (argc, argv);
		QtEncodingMainWindow*	window	= new QtEncodingMainWindow (0);
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

