/**
 * @file		QtMultiLineToolbar.cpp
 * @author		Charles PIGNEROL
 * @date		13/01/2015
 */

#include "QtUtil/QtWidgetAction.h"

#include "QtUtil/QtMultiLineToolbar.h"
#include "QtUtil/QtFlowLayout.h"

#include <TkUtil/MemoryError.h>

#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QAbstractButton>
#else	// QT_5
#include <QApplication>
#include <QAbstractButton>
#endif	// QT_5

#include <assert.h>


QtMultiLineToolbar::QtMultiLineToolbar (QWidget* parent)
	: QWidget (parent), _iconSize (32, 32)
{
	setLayout (new QtFlowLayout (this));
	QApplication*	app	=
				dynamic_cast<QApplication*>(QApplication::instance ( ));
	if ((0 != app) && (0 != app->style ( )))
	{
		int	size	= app->style ( )->pixelMetric (QStyle::PM_ToolBarIconSize);
		_iconSize	= QSize (size, size);
	}
}	// QtMultiLineToolbar::QtMultiLineToolbar


QtMultiLineToolbar::QtMultiLineToolbar (const QtMultiLineToolbar&)
	: QWidget (0), _iconSize (32, 32)
{
	assert (0 && "QtMultiLineToolbar::QtMultiLineToolbar is not allowed.");
}	// QtMultiLineToolbar::QtMultiLineToolbar (const QtMultiLineToolbar&)


QtMultiLineToolbar& QtMultiLineToolbar::operator = (const QtMultiLineToolbar&)
{
	assert (0 && "QtMultiLineToolbar::operator = is not allowed.");
	return *this;
}	// QtMultiLineToolbar::operator =


QtMultiLineToolbar::~QtMultiLineToolbar ( )
{
}	// QtMultiLineToolbar::~QtMultiLineToolbar


void QtMultiLineToolbar::addAction (QWidgetAction& action)
{
	CHECK_NULL_PTR_ERROR (layout ( ))
	QWidget*	icon	= action.requestWidget (this);
	CHECK_NULL_PTR_ERROR (icon)
	QAbstractButton*	button	= dynamic_cast<QAbstractButton*>(icon);
	if (0 != button)
		button->setIconSize (iconSize ( ));
	else
		icon->setFixedSize (iconSize ( ));
	layout ( )->addWidget (icon);
}	// QtMultiLineToolbar::addAction


void QtMultiLineToolbar::addSeparator ( )
{
	CHECK_NULL_PTR_ERROR (layout ( ))
	QWidget*	separator	= new QWidget (this);
	separator->setFixedSize (
			QSize (iconSize ( ).width ( ) / 2, iconSize ( ).height ( ) / 2));
	layout ( )->addWidget (separator);
}	// QtMultiLineToolbar::addSeparator


const QSize& QtMultiLineToolbar::iconSize ( ) const
{
	return _iconSize;
}	// QtMultiLineToolbar::iconSize


void QtMultiLineToolbar::setIconSize (const QSize& size)
{
	if (size != _iconSize)
	{
		_iconSize	= size;

		CHECK_NULL_PTR_ERROR (layout ( ))
		const int	count	= layout ( )->count ( );
		QSize		sepSize (size.width ( ) / 2, size.height ( ) / 2);
		for (int i = 0; i < count; i++)
		{
			QLayoutItem*	item	= layout ( )->itemAt (i);
			CHECK_NULL_PTR_ERROR (item)
			QWidget*			w		= item->widget ( );
			CHECK_NULL_PTR_ERROR (w)
			QAbstractButton*	button	= dynamic_cast<QAbstractButton*>(w);
			if (0 != button)
				button->setIconSize (iconSize ( ));
			else
				w->setFixedSize (sepSize);
		}	// for (int i = 0; i < count; i++)
	}	// if (size != _iconSize)
}	// QtMultiLineToolbar::setIconSize


