#include "PrefsQt/QtPrefsNamedValueListViewItem.h"
#include <QtUtil/QtObjectSignalBlocker.h>

#include <assert.h>


USING_STD
USING_PREFS


QtPrefsNamedValueListViewItem::QtPrefsNamedValueListViewItem (
	QtPrefsSectionListViewItem* parent, NamedValue* namedValue, bool editable)
	: QtPrefsListViewItem (parent, namedValue, editable)
{
	assert ((0 != namedValue) && "QtPrefsNamedValueListViewItem::QtPrefsNamedValueListViewItem : null named value.");
	QtObjectSignalBlocker	blocker (treeWidget ( )->model ( ));	// v 5.7.0
	setText (1, namedValue->getStrValue ( ).c_str ( ));
	if (true == editable)
		setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}	// QtPrefsNamedValueListViewItem::QtPrefsNamedValueListViewItem


QtPrefsNamedValueListViewItem::QtPrefsNamedValueListViewItem (
					const QtPrefsNamedValueListViewItem&)
	: QtPrefsListViewItem ((QTreeWidget*)0, 0, false)
{
	assert (0 && "QtPrefsNamedValueListViewItem copy constructor is not allowed.");
}	// QtPrefsNamedValueListViewItem::QtPrefsNamedValueListViewItem (const QtPrefsNamedValueListViewItem&)


QtPrefsNamedValueListViewItem& QtPrefsNamedValueListViewItem::operator =(const QtPrefsNamedValueListViewItem&)
{
	assert (0 && "QtPrefsNamedValueListViewItem operator = is not allowed.");
	return *this;
}	// QtPrefsNamedValueListViewItem::operator =


QtPrefsNamedValueListViewItem::~QtPrefsNamedValueListViewItem ( )
{
}	// QtPrefsNamedValueListViewItem::~QtPrefsNamedValueListViewItem


void QtPrefsNamedValueListViewItem::valueModified ( )
{
	string	value (text (1).toStdString ( ));
	getNamedValue ( ).setStrValue (value);
}	// QtPrefsNamedValueListViewItem::valueModified


NamedValue& QtPrefsNamedValueListViewItem::getNamedValue ( ) const
{
	NamedValue*	namedValue	= dynamic_cast<NamedValue*>(&getElement ( ));
	assert ((0 != namedValue) && "QtPrefsNamedValueListViewItem::getNamedValue : null section.");
	return *namedValue;
}	// QtPrefsNamedValueListViewItem::getNamedValue



