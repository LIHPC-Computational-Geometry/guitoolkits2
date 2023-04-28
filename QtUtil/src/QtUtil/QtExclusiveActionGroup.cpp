#include "QtUtil/QtExclusiveActionGroup.h"
#include "TkUtil/Exception.h"

#ifndef QT_5
#include <QtGui/QMenu>
#else	// QT_5
#include <QMenu>
#endif	// QT_5

#include <assert.h>


USING_UTIL


QtExclusiveActionGroup::QtExclusiveActionGroup (QObject* parent)
	: QAction (parent), _actionGroup (0), _currentAction (-1)
{
	QMenu*	menu	= new QMenu (0);
	setMenu (menu);
	menu->setToolTipsVisible (true);
	_actionGroup	= new QActionGroup (this);
	_actionGroup->setExclusive (true);
	connect (_actionGroup, SIGNAL (triggered (QAction*)), this,
	         SLOT (currentSelectionCallback (QAction*)));
}	// QtExclusiveActionGroup::QtExclusiveActionGroup


QtExclusiveActionGroup::QtExclusiveActionGroup (const QtExclusiveActionGroup&)
	: QAction (0), _actionGroup (0), _currentAction (-1)
{
	assert (0 && "QtExclusiveActionGroup::QtExclusiveActionGroup is not allowed.");
}	// QtExclusiveActionGroup::QtExclusiveActionGroup (const QtExclusiveActionGroup&)


QtExclusiveActionGroup& QtExclusiveActionGroup::operator = (
												const QtExclusiveActionGroup&)
{
	assert (0 && "QtExclusiveActionGroup::operator = is not allowed.");
	return *this;
}	// QtExclusiveActionGroup::operator =


QtExclusiveActionGroup::~QtExclusiveActionGroup ( )
{
}	// QtExclusiveActionGroup::~QtExclusiveActionGroup


void QtExclusiveActionGroup::addAction (const QIcon& icon, const QString& text)
{
	assert (0 != _actionGroup);
	assert (0 != menu ( ));
	QAction*	action	= new QAction (icon, text, this);
	action->setCheckable (true);
	action->setActionGroup (_actionGroup);
	menu ( )->addAction (action);
}	// QtExclusiveActionGroup::addAction


void QtExclusiveActionGroup::setCurrentIndex (int index)
{
	assert (0 != _actionGroup);
	if (index == _currentAction)
		return;

	QList<QAction*>	actions	= _actionGroup->actions ( );
	if (-1 == index)
	{
		if (actions.size ( ) > _currentAction)
			actions [_currentAction]->setChecked (false);
		setIcon (QIcon ( ));
		setText ("");
	}
	else
	{
		if (actions.size ( ) <= index)
			throw Exception ("QtExclusiveActionGroup::setCurrentIndex : index hors domaine.");
		actions [index]->setChecked (true);
		setIcon (actions [index]->icon ( ));
		setText (actions [index]->text ( ));
	}

	_currentAction	= index;
}	// QtExclusiveActionGroup::setCurrentIndex


void QtExclusiveActionGroup::currentSelectionCallback (QAction* action)
{
	assert (0 != _actionGroup);
	const QList<QAction*>	actions	= _actionGroup->actions ( );
	int						i		= 0;
	for (QList<QAction*>::const_iterator it = actions.begin ( );
	     actions.end ( ) != it; it++, i++)
		if (*it == action)
		{
			setIcon (action->icon ( ));
			setText (action->text ( ));
			_currentAction	= i;
			emit activated (_currentAction);
			break;
		}	// if (*it == action)
}	// QtExclusiveActionGroup::currentSelectionCallback


