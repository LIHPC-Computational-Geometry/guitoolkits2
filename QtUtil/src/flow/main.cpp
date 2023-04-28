#ifndef QT_5
#include <QtGui/QApplication>
#else	// QT_5
#include <QtWidgets/QApplication>
#endif	// QT_5

#include "window.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE (Flow);

	QApplication	app (argc, argv);
	Window			window;
	window.show ( );

	return app.exec ( );
}
