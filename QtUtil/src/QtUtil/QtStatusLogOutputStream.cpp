#include "QtUtil/QtStatusLogOutputStream.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/MemoryError.h>

#include <assert.h>

USING_UTIL


QtStatusLogOutputStream::QtStatusLogOutputStream (
		QStatusBar* statusBar, Log::TYPE mask, bool enableDate, bool enableTime)
	: QObject ( ),
	  DefaultLogOutputStream (mask, enableDate, enableTime),
	  _statusBar (statusBar)
{
	if (0 != _statusBar)
	{
		// On connecte la méthode log à showMessage de la barre de status
		// => provoque un changement de thread salvateur lorsque log est appelé
		// depuis un thread autre que celui de la barre de status.
		QObject::connect (this, SIGNAL (logCalled (QString)), _statusBar,
		         SLOT (showMessage (QString)));
	}	// if (0 != _statusBar)
}	// QtStatusLogOutputStream::QtStatusLogOutputStream


QtStatusLogOutputStream::QtStatusLogOutputStream (
											const QtStatusLogOutputStream& view)
{
	assert (0 && "QtStatusLogOutputStream::QtStatusLogOutputStream copy constructor is not allowed.");
}	// QtStatusLogOutputStream::QtStatusLogOutputStream (const QtStatusLogOutputStream& view)


QtStatusLogOutputStream& QtStatusLogOutputStream::operator = (
											const QtStatusLogOutputStream& view)
{
	assert (0 && "QtStatusLogOutputStream::operator = is not allowed.");
	return *this;
}	// QtStatusLogOutputStream::QtStatusLogOutputStream (const QtStatusLogOutputStream& view)


QtStatusLogOutputStream::~QtStatusLogOutputStream ( )
{
}	// QtStatusLogOutputStream::~QtStatusLogOutputStream


void QtStatusLogOutputStream::log (const Log& log)
{
#ifdef MULTITHREADED_APPLICATION
	LOCK_LOG_STREAM
#endif	// MULTITHREADED_APPLICATION

	QString	message (UTF8TOQSTRING (log.getText ( )));
	emit logCalled (message);
	// On ne fait rien : le signal émis provoquera l'affichage du message.
}	// QtStatusLogOutputStream::log


