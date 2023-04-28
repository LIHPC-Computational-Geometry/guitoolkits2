#include "QtUtil/QtMultiLineToolbar.h"
#include "QtUtil/QtWidgetAction.h"
#include "QtUtil/QtFlowLayout.h"
#include "window.h"

#include <QtGui/QtGui>
#ifndef QT_5
#include <QtGui/QPushButton>
#else	// QT_5
#include <QtWidgets/QPushButton>
#endif	// QT_5
#include <iostream>

using namespace std;


Window::Window()
{
	QVBoxLayout*	vlayout	= new QVBoxLayout ( );
	QtMultiLineToolbar*	tb	= new QtMultiLineToolbar (this);
	vlayout->addWidget (tb);
	QtWidgetAction*	action	=
		new QtWidgetAction (QIcon (":/images/undo.png"), "Annuler", tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (undoCallback ( )));
	tb->addAction (*action);
	action	=
		new QtWidgetAction (QIcon (":/images/redo.png"), "Refaire", tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (redoCallback ( )));
	tb->addAction (*action);
	tb->addSeparator ( );
	action	=
		new QtWidgetAction (QIcon (":/images/print.png"), "Imprimer ...", tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (printCallback ( )));
	tb->addAction (*action);
	tb->addSeparator ( );
	action	=
		new QtWidgetAction (QIcon (":/images/palette.png"), "Graphique ...",tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (paletteCallback ( )));
	tb->addAction (*action);
	tb->addSeparator ( );
	action	=
		new QtWidgetAction (QIcon (":/images/dash_dot_dot_line.png"), "DDDL", tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (dashDotDotLineCallback ( )));
	tb->addAction (*action);
	action	=
		new QtWidgetAction (QIcon (":/images/dash_dot_line.png"), "DDL",tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (dashDotLineCallback ( )));
	tb->addAction (*action);
	action	=
		new QtWidgetAction (QIcon (":/images/dash_line.png"), "DL",tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (dashLineCallback ( )));
	tb->addAction (*action);
	action	=
		new QtWidgetAction (QIcon (":/images/dot_line.png"), "DL",tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (dotLineCallback ( )));
	tb->addAction (*action);
	tb->addSeparator ( );
	action	=
		new QtWidgetAction (QIcon (":/images/zoomin.png"), "Zoomer",tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (zoomInCallback ( )));
	tb->addAction (*action);
	action	=
		new QtWidgetAction (QIcon (":/images/zoomout.png"), "Dézoomer",tb);
	connect (action, SIGNAL (triggered ( )), this, SLOT (zoomOutCallback ( )));
	tb->addAction (*action);
    QtFlowLayout *flowLayout = new QtFlowLayout;

	flowLayout->addWidget(new QPushButton(tr("Short")));
	flowLayout->addWidget(new QPushButton(tr("Longer")));
	flowLayout->addWidget(new QPushButton(tr("Different text")));
	flowLayout->addWidget(new QPushButton(tr("More text")));
	flowLayout->addWidget(new QPushButton(tr("Even longer button text")));
	vlayout->addLayout (flowLayout);
	setLayout (vlayout);

    setWindowTitle(tr("Flow Layout"));
}

void Window::redoCallback ( )
{
cout << "Window::redoCallback called." << endl;
}
void Window::undoCallback ( )
{
cout << "Window::undoCallback called." << endl;
}
void Window::dashDotDotLineCallback ( )
{
cout << "Window::dashDotDotLineCallback called." << endl;
}
void Window::dashDotLineCallback ( )
{
cout << "Window::dashDotLineCallback called." << endl;
}
void Window::dashLineCallback ( )
{
cout << "Window::dashLineCallback called." << endl;
}
void Window::dotLineCallback ( )
{
cout << "Window::dotLineCallback called." << endl;
}
void Window::paletteCallback ( )
{
cout << "Window::paletteCallback called." << endl;
}
void Window::printCallback ( )
{
cout << "Window::printCallback called." << endl;
}
void Window::zoomInCallback ( )
{
cout << "Window::zoomInCallback called." << endl;
}
void Window::zoomOutCallback ( )
{
cout << "Window::zoomOutCallback called." << endl;
}
