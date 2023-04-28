/**
 * @file		QtWidgetAction.cpp
 * @author		Charles PIGNEROL
 * @date		13/01/2015
 */

#include "QtUtil/QtWidgetAction.h"

#ifndef QT_5
#include <QtGui/QToolButton>
#else	// QT_5
#include <QToolButton>
#endif	// QT_5

#include <assert.h>



QtWidgetAction::QtWidgetAction (QWidget* parent)
	: QWidgetAction (parent)
{
}	// QtWidgetAction::QtWidgetAction


QtWidgetAction::QtWidgetAction (
						const QIcon& icon, const QString& text, QObject* parent)
	: QWidgetAction (parent)
{
	setIcon (icon);
	setText (text);
}	// QtWidgetAction::QtWidgetAction


QtWidgetAction::QtWidgetAction (const QtWidgetAction&)
	: QWidgetAction (0)
{
	assert (0 && "QtWidgetAction::QtWidgetAction is not allowed.");
}	// QtWidgetAction::QtWidgetAction (const QtWidgetAction&)


QtWidgetAction& QtWidgetAction::operator = (const QtWidgetAction&)
{
	assert (0 && "QtWidgetAction::operator = is not allowed.");
	return *this;
}	// QtWidgetAction::operator =


QtWidgetAction::~QtWidgetAction ( )
{
}	// QtWidgetAction::~QtWidgetAction


QWidget* QtWidgetAction::createWidget (QWidget* parent)
{
	QToolButton*	button	= new QToolButton (parent);
	button->setDefaultAction (this);

	return button;
}	// QtWidgetAction::createWidget


