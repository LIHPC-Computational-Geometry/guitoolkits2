#include "PrefsQt/QtPrefsSectionListViewItem.h"
#include "PrefsQt/QtPrefsNamedValueListViewItem.h"
#include "PrefsQt/QtXMLNameValidator.h"
#include <QtUtil/QtUnicodeHelper.h>
#include <TkUtil/Exception.h>

#include <assert.h>


USING_STD
USING_UTIL
USING_PREFS


QtPrefsSectionListViewItem::QtPrefsSectionListViewItem (
			QTreeWidget* parent, Section* section, bool editable)
	: QtPrefsListViewItem (parent, section, editable)
{
}	// QtPrefsSectionListViewItem::QtPrefsSectionListViewItem


QtPrefsSectionListViewItem::QtPrefsSectionListViewItem (
		QtPrefsSectionListViewItem* parent, Section* section, bool editable)
	: QtPrefsListViewItem (parent, section, editable)
{
}	// QtPrefsSectionListViewItem::QtPrefsSectionListViewItem


QtPrefsSectionListViewItem::QtPrefsSectionListViewItem (
					const QtPrefsSectionListViewItem&)
	: QtPrefsListViewItem ((QTreeWidget*)0, 0, false)
{
	assert (0 && "QtPrefsSectionListViewItem copy constructor is not allowed.");
}	// QtPrefsSectionListViewItem::QtPrefsSectionListViewItem (const QtPrefsSectionListViewItem&)


QtPrefsSectionListViewItem& QtPrefsSectionListViewItem::operator =(const QtPrefsSectionListViewItem&)
{
	assert (0 && "QtPrefsSectionListViewItem operator = is not allowed.");
	return *this;
}	// QtPrefsSectionListViewItem::operator =


QtPrefsSectionListViewItem::~QtPrefsSectionListViewItem ( )
{
}	// QtPrefsSectionListViewItem::~QtPrefsSectionListViewItem


void QtPrefsSectionListViewItem::nameModified ( )
{
	QString	name (text (0));
	if (false == getSection ( ).hasParent ( ))	
	{
		if (0 != getNameValidator ( ))
		{
			int	pos	= 0;
			if (QValidator::Invalid == 
					getNameValidator ( )->validate (name, pos))
				 throw Exception ("Nom de section invalide.");
		}	// if (0 != getNameValidator ( ))

		getSection ( ).rename (QtUnicodeHelper::qstringToUnicodeString (name));
	}
	else
		QtPrefsListViewItem::nameModified ( );
}	// QtPrefsSectionListViewItem::nameModified


Section& QtPrefsSectionListViewItem::getSection ( ) const
{
	Section*	section	= dynamic_cast<Section*>(&getElement ( ));
	assert ((0 != section) && "QtPrefsSectionListViewItem::getSection : null section.");
	return *section;
}	// QtPrefsSectionListViewItem::getSection


void QtPrefsSectionListViewItem::createChildren ( )
{
	Section&	section	= getSection ( );

	vector<Section*>	subsections	= section.getSections ( );
	for (vector<Section*>::iterator its = subsections.begin ( ); subsections.end ( ) != its; its++)
	{
		const bool	editable	= (true == isEditable ( )) && (true == (*its)->isOverloadable ( )) ? true : false;	// v 5.7.0
		QtPrefsSectionListViewItem*	item	= new QtPrefsSectionListViewItem (this, *its, editable);
		item->createChildren ( );
		item->setNameValidator (new QtXMLNameValidator (0));
		insertItem (item);
	}	// for (vector<Section*>::iterator its = subsections.begin ( );

	vector<NamedValue*>	namedValues	= section.getNamedValues ( );
	for (vector<NamedValue*>::iterator itnv = namedValues.begin ( ); namedValues.end ( ) != itnv; itnv++)
	{
		const bool	editable	= (true == isEditable ( )) && (true == (*itnv)->isOverloadable ( )) ? true : false;	// v 5.7.0
		QtPrefsNamedValueListViewItem*	item	= new QtPrefsNamedValueListViewItem (this, *itnv, editable);
		item->setNameValidator (new QtXMLNameValidator (0));
		insertItem (item);
	}	// for (vector<NamedValue*>::iterator itnv = namedValues.begin ( );
}	// QtPrefsSectionListViewItem::createChildren


