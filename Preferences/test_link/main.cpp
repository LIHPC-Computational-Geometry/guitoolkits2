#include "PrefsQt/QtPreferencesDialog.h"

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
	bool	editable = true;
	string	fileName;

    QApplication	application (argc, argv);
    Section*		section	= 0;
	QtPreferencesDialog* dialog = new QtPreferencesDialog (0, section, fileName, editable, false, true, "Config Editor", "Config Editor");

    // for CI, do not call exec()
    // dialog->exec ( );
    delete dialog;

	return 0;
}	// main
