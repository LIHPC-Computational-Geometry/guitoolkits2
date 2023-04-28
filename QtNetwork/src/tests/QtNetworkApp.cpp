#include <iostream>

#include <QtUtil/QtUnicodeHelper.h>

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QMenu>
#include <QUrl>

#include "QtNetworkApp.h"
#include "QtNetWork/QtNetworkFileDialog.h"
#include "TkUtil/NetworkData.h"
#include "TkUtil/UserData.h"
#include "TkUtil/RemoteProcess.h"


USING_STD
USING_UTIL

static const TkUtil::Charset	charset ("àéèùô");

USE_ENCODING_AUTODETECTION


QtNetworkApp::QtNetworkApp (int& argc, char** argv)
	: QApplication (argc, argv)
{
	RemoteProcess::remoteShell	= "ssh";
	RemoteProcess::launcher		= "tkutil_launcher";;

	QMainWindow*	mainWindow	= new QMainWindow (0);
	mainWindow->setWindowTitle ("NETWORK APP");
	QMenuBar*		menuBar		= mainWindow->menuBar ( );
	QMenu*		menu			= new QMenu ("&Fichier", mainWindow);
	menuBar->addMenu (menu);
	menu->setToolTipsVisible (true);
	menu->addAction (QSTR ("Ouvrir ..."), this, SLOT(open ( )));
	menu->addAction (QSTR ("Ouvrir sur le réseau ..."), this, SLOT(openNetwork ( )));
	mainWindow->show ( );
}	// QtNetworkApp::QtNetworkApp


void QtNetworkApp::open ( )
{
	const string	filter ("*.xml *.html *.txt");
	static QFileDialog	dialog (
						0, QSTR ("Sélectionnez un fichier"),
						UserData ( ).getHome ( ).c_str ( ), filter.c_str ( ));
	dialog.setFileMode (QFileDialog::ExistingFiles);

	if (QDialog::Accepted == dialog.exec ( ))
	{
		QStringList	fileList	= dialog.selectedFiles ( );

		cout << "TO OPEN : " << fileList [0].toStdString ( ) << endl;
	}	// if (QDialog::Accepted == dialog.exec ( ))
}	// QtNetworkApp::open


void QtNetworkApp::openNetwork ( )
{
	static string	lastHost (NetworkData::getCurrentHostName ( ));
	const string	textFilter ("Text (*.xml *.html *.txt)");
	const string	otherFilters ("Maillages (*.unv *.uns *.ice) ; Office (*.doc *.xls *.ppt);*.tgz ");
	static QtNetworkFileDialog	dialog (
						lastHost.c_str ( ), UserData ( ).getHome ( ).c_str ( ), "*",
						NULL, "Sélectionnez un fichier", true, true);
	bool				first	= true;
	if (true == first)
	{
		QStringList	filters;
		filters.push_back (textFilter.c_str ( ));
		filters.push_back (otherFilters.c_str ( ));
		filters.push_back ("Tous (*.*)");
		dialog.setNameFilters (filters);
		dialog.selectNameFilter (textFilter.c_str ( ));
		dialog.setFileMode (QFileDialog::ExistingFiles);
		first	= false;
	}	// if (true == first)
	QStringList	dirs;
	dirs << "/home/charles/Documents" << "/home/charles/OLD" << "/home/charles/projets";
	dialog.addPreselectedDirectories (dirs);
	dialog.freeze (false);

	if (QDialog::Accepted == dialog.exec ( ))
	{
		QStringList	fileList	= dialog.selectedFiles ( );
		lastHost	= dialog.getHostName ( ).toStdString ( );

		cout << "TO OPEN : ";
		for (QStringList::Iterator it = fileList.begin ( );
		     (fileList.end ( ) != it); it++)
		{
			const QUrl		url (*it);
			const string	fileName	= url.path ( ).toStdString ( );
			cout << fileName << "@" << lastHost << ' ';
		}	// for (QStringList::Iterator it = fileList.begin ( );
		cout << endl;
	}	// if (QDialog::Accepted == dialog.exec ( ))
}	// QtNetworkApp::openNetwork

