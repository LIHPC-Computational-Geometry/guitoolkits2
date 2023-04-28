#include "QtUtil/QtLogsView.h"
#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtEncodingMainWindow.h"
#include "QtUtil/QtStatusLogOutputStream.h"
#include "Iso8859_1.h"
#include "UTF_8.h"

#include <TkUtil/InformationLog.h>
#include <TkUtil/LogDispatcher.h>
#include <TkUtil/OstreamLogOutputStream.h>

#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>

using namespace TkUtil;

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtEncodingMainWindow::QtEncodingMainWindow (QWidget* parent)
	: QMainWindow (parent)
{
	QMenu*	menu	= new QMenu ("Application", this);
	menuBar ( )->addMenu (menu);
	menu->setToolTipsVisible (true);
	menu->addAction ("Quitter", this, SLOT (exitCallback ( )));

	QWidget*	centralWidget	= new QWidget (this);
	QBoxLayout*	layout			= new QVBoxLayout ( );
	centralWidget->setLayout (layout);
	setCentralWidget (centralWidget);
	QLabel*	label	= new QLabel (fromIso8859_1 ( ));
	layout->addWidget (label);
	label	= new QLabel (fromUTF8 ( ));
	layout->addWidget (label);
	QtLogsView*	logView	= new QtLogsView (0, Log::ALL_KINDS);
	layout->addWidget (logView);
	QtStatusLogOutputStream*	statusView	=
				new QtStatusLogOutputStream (statusBar ( ), Log::ALL_KINDS);

	// Ugly code :
	LogDispatcher*	dispatcher	= new LogDispatcher ( );
	dispatcher->addStream (logView);
	dispatcher->addStream (statusView);
	OstreamLogOutputStream*	stdoutLogStream	=
								new OstreamLogOutputStream (std::cout);
	stdoutLogStream->setCharset (charset);
	dispatcher->addStream (stdoutLogStream);

	// Passe par DefaultLog (string) => mauvais encodage
	dispatcher->log (InformationLog (iso8859_1Str ( )));
	// Passe par DefaultLog (UTF8String) => bon encodage
	dispatcher->log (InformationLog (isoStr ( )));
	// Passe par DefaultLog (string) => mauvais encodage
	dispatcher->log (InformationLog (utf8Str ( )));
	// Passe par DefaultLog (UTF8String) => bon encodage
	dispatcher->log (InformationLog (utfStr ( )));
}	// QtEncodingMainWindow::QtEncodingMainWindow


#ifdef QT_4
QtEncodingMainWindow::QtEncodingMainWindow (const QtEncodingMainWindow&)
	: QMainWindow (0)
{
}	// QtEncodingMainWindow::QtEncodingMainWindow
#endif	// QT_4


QtEncodingMainWindow& QtEncodingMainWindow::operator = (
													const QtEncodingMainWindow&)
{
	return *this;
}	// QtEncodingMainWindow::operator =


QtEncodingMainWindow::~QtEncodingMainWindow ( )
{
}	// QtEncodingMainWindow::~QtEncodingMainWindow


void QtEncodingMainWindow::exitCallback ( )
{
	switch (QtMessageBox::displayQuestionMessage (
			this, "QUITTER",
			UTF8STR ("Souhaitez-vous réellement quitter l'application ?"), 100,
			"OK", "Annuler", 0, 0))
	{
		case	0	: QApplication::exit (0);
			break;
	}
}	// QtEncodingMainWindow::exitCallback


