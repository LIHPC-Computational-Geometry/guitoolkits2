#include "PrefsQt/QtPrefsListViewItem.h"
#include "PrefsQt/QtPrefsSectionListViewItem.h"
#include <QtUtil/QtObjectSignalBlocker.h>
#include <QtUtil/QtUnicodeHelper.h>
#include <TkUtil/Exception.h>

#include <assert.h>


USING_STD
USING_UTIL
USING_PREFS


QtPrefsListViewItem::QtPrefsListViewItem (QTreeWidget* parent, Element* element, bool editable)
	: QTreeWidgetItem (parent), _element (element), _editable (editable), _nameValidator (0)
{
	assert ((0 != element) && "QtPrefsListViewItem::QtPrefsListViewItem : null element.");
	QtObjectSignalBlocker	blocker (treeWidget ( )->model ( ));	// v 5.7.0
	setText (0, UTF8TOQSTRING (_element->getName( )));
	setText (2, UTF8TOQSTRING (_element->getComment ( )));
	if (true == editable)
		setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	else
//		setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setFlags (Qt::ItemIsSelectable);	// v 5.7.0 => grayed
}	// QtPrefsListViewItem::QtPrefsListViewItem


QtPrefsListViewItem::QtPrefsListViewItem (QtPrefsSectionListViewItem* parent, Element* element, bool editable)
	: QTreeWidgetItem (parent), _element (element), _editable (editable), _nameValidator (0)
{
	assert ((0 != element) && "QtPrefsListViewItem::QtPrefsListViewItem : null element.");
	QtObjectSignalBlocker	blocker (treeWidget ( )->model ( ));	// v 5.7.0
	setText (0, UTF8TOQSTRING (_element->getName( )));
	setText (2, UTF8TOQSTRING (_element->getComment ( )));
	if (true == editable)
		setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	else
//		setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setFlags (Qt::ItemIsSelectable);	// v 5.7.0 => grayed
}	// QtPrefsListViewItem::QtPrefsListViewItem


QtPrefsListViewItem::QtPrefsListViewItem (const QtPrefsListViewItem&)
	: QTreeWidgetItem ((QTreeWidget*)0)
{
	assert (0 && "QtPrefsListViewItem copy constructor is not allowed.");
}	// QtPrefsListViewItem::QtPrefsListViewItem (const QtPrefsListViewItem&)


QtPrefsListViewItem& QtPrefsListViewItem::operator =(const QtPrefsListViewItem&)
{
	assert (0 && "QtPrefsListViewItem operator = is not allowed.");
	return *this;
}	// QtPrefsListViewItem::operator =


QtPrefsListViewItem::~QtPrefsListViewItem ( )
{
	if ((0 != getNameValidator ( )) && (0 == getNameValidator ( )->parent ( )))
		delete getNameValidator ( );
}	// QtPrefsListViewItem::~QtPrefsListViewItem


Element& QtPrefsListViewItem::getElement ( ) const
{
	assert ((0 != _element) && "QtPrefsListViewItem::getElement : null element.");
	return *_element;
}	// QtPrefsListViewItem::getElement


void QtPrefsListViewItem::nameModified ( )
{
	QString	name (text (0));

	if (0 != getNameValidator ( ))
	{
		int	pos	= 0;
		if (QValidator::Invalid == getNameValidator ( )->validate (name, pos))
			throw Exception ("Nom d'élément invalide.");
	}	// if (0 != getNameValidator ( ))


	if (true == getElement ( ).hasParent ( ))
		getElement ( ).getParent ( ).renameElement (getElement ( ).getName ( ), QtUnicodeHelper::qstringToUTF8String (name));
}	// QtPrefsListViewItem::nameModified



void QtPrefsListViewItem::setComment (const UTF8String& comment)
{
	setText (2, UTF8TOQSTRING (comment));
	commentModified ( );
}	// QtPrefsListViewItem::setComment


void QtPrefsListViewItem::commentModified ( )
{
	getElement ( ).setComment (QtUnicodeHelper::qstringToUTF8String (text (2)));
}	// QtPrefsListViewItem::commentModified


QValidator* QtPrefsListViewItem::getNameValidator ( ) const
{
	return _nameValidator;
}	// QtPrefsListViewItem::getNameValidator


void QtPrefsListViewItem::setNameValidator (QValidator* validator)
{
	if ((0 != _nameValidator) && (0 == _nameValidator->parent ( )))
		delete _nameValidator;

	_nameValidator	= validator;
}	// QtPrefsListViewItem::setNameValidator

