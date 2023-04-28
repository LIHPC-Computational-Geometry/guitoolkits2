#include "PrefsQt/QtPreferencesPanel.h"
#include "PrefsQt/QtPrefsSectionListViewItem.h"
#include "PrefsQt/QtPrefsNamedValueListViewItem.h"
#include "PrefsQt/QtXMLNameValidator.h"
#include "PrefsCore/BoolNamedValue.h"
#include "PrefsCore/ColorNamedValue.h"
#include "PrefsCore/DoubleNamedValue.h"
#include "PrefsCore/LongNamedValue.h"
#include "PrefsCore/StringNamedValue.h"
#include "PrefsCore/UnsignedLongNamedValue.h"
#include <TkUtil/Exception.h>
#include <QtUtil/QtUnicodeHelper.h>
#include <QtUtil/QtTextDialog.h>
#include <QtUtil/QtMessageBox.h>

#include <QCursor>
#ifndef QT_5
#include <QtGui/QApplication>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>
#else	// QT_5
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#endif	// QT_5

#include <assert.h>
#include <memory>


USING_STD
USING_UTIL
USING_PREFS


static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


#define CHECK_IF_EDITABLE(message)                                            \
	{                                                                         \
		if (false == isEditable ( ))                                          \
		{                                                                     \
			UTF8String	errorMsg (charset);                                   \
			errorMsg << #message << "\nConfiguration non éditable.";          \
			displayErrorMessage (errorMsg);                                   \
			return;                                                           \
		}                                                                     \
	}

#define CHECK_SELECTION(message)                                              \
	{                                                                         \
		QtPrefsListViewItem*	item	=                                     \
				dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));         \
	                                                                          \
		if (0 == item)                                                        \
		{                                                                     \
			UTF8String	errorMsg (charset);                                   \
			errorMsg << #message <<" absence d'élement sélectionné.";         \
			displayErrorMessage (errorMsg);                                   \
			return;                                                           \
		}	/* if (0 == item) */                                              \
	}


QtPreferencesPanel::QtPreferencesPanel (
	QWidget* parent, Section* section, bool editable, const string& appTitle)
	: QTreeWidget (parent),
	   _mainSection (section), _editable (editable), _appTitle (appTitle)
{
	setSelectionMode (QTreeWidget::SingleSelection);
	setRootIsDecorated (true);
	QAbstractItemModel*	treeModel	= model ( );
	assert (0 != treeModel);
	treeModel->insertColumns (0, 2);
	treeModel->setHeaderData (0, Qt::Horizontal, QVariant (QSTR ("Données")));
	treeModel->setHeaderData (1, Qt::Horizontal, QVariant ("Valeurs"));
	treeModel->setHeaderData (2, Qt::Horizontal, QVariant ("Commentaires"));
	setSortingEnabled (false);
	connect (this, SIGNAL (itemChanged (QTreeWidgetItem*, int)), this,
	         SLOT (elementRenamed (QTreeWidgetItem*, int)));

	if (0 != section)
	{
		QtPrefsSectionListViewItem*	rootItem	= 
				new QtPrefsSectionListViewItem (this, _mainSection, editable);
		rootItem->createChildren ( );
		rootItem->setNameValidator (new QtXMLNameValidator (0));
		insertTopLevelItem (0, rootItem);
		// On essaye d'ouvrir un peu le panneau afin d'avoir des colonnes
		// dimensionnées a minima :
		expandItem (rootItem);
		int	width		= 50;
		int	rootWidth	= 50;
		for (int i = 0; i < rootItem->childCount ( ); i++)
		{
			QTreeWidgetItem*	item	= rootItem->child (i);
			expandItem (item);
			width		= width < sizeHintForColumn (1) ?
						  sizeHintForColumn (1) : width;
			rootWidth	= rootWidth < sizeHintForColumn (0) ?
						  sizeHintForColumn (0) : rootWidth;
			collapseItem (item);
		}	// for (int i = 0; i < rootItem->childCount ( ); i++)
		setColumnWidth (0, rootWidth);
		setColumnWidth (1, width);
		setColumnWidth (2, 100);
	}	// if (0 != section)
}	// QtPreferencesPanel::QtPreferencesPanel


QtPreferencesPanel::QtPreferencesPanel (const QtPreferencesPanel&)
	: QTreeWidget (0),
	   _mainSection (0), _appTitle ("")
{
	assert (0 && "QtPreferencesPanel copy constructor is not allowed.");
}	// QtPreferencesPanel::QtPreferencesPanel


QtPreferencesPanel& QtPreferencesPanel::operator = (const QtPreferencesPanel&)
{
	assert (0 && "QtPreferencesPanel operator = is not allowed.");
	return *this;
}


QtPreferencesPanel::~QtPreferencesPanel ( )
{
	delete _mainSection;
}	// QtPreferencesPanel::~QtPreferencesPanel


bool QtPreferencesPanel::hasSection ( ) const
{
	return 0 == _mainSection ? false : true;
}	// QtPreferencesPanel::hasSection


const Section& QtPreferencesPanel::getSection ( ) const
{
	if (0 == _mainSection)
		throw Exception ("Absence de configuration chargée.");

	return *_mainSection;
}	// QtPreferencesPanel::getSection


Section* QtPreferencesPanel::releaseSection ( )
{
	QTreeWidgetItem*	treeWidget	= takeTopLevelItem (0);
	delete treeWidget;
	assert (0 == takeTopLevelItem (0));

	Section*	section	= _mainSection;
	_mainSection		= 0;
	return section;
}	// QtPreferencesPanel::releaseSection


void QtPreferencesPanel::setRootSection (Section* section)
{
	Section*	oldSection	= releaseSection ( );
	delete oldSection;		oldSection	= 0;

	if (0 != section)
	{
		QtPrefsSectionListViewItem*	rootItem	= 
				new QtPrefsSectionListViewItem (this, section, isEditable ( ));
		rootItem->createChildren ( );
		rootItem->setNameValidator (new QtXMLNameValidator (0));
		insertTopLevelItem (0, rootItem);
		// v 2.29.1 : ajuster la taille des colonnes :
		rootItem->setExpanded (true);
		const int	minWidth	= 150;
		int	width0	= sizeHintForColumn (0);
		int	width1	= sizeHintForColumn (1);
		width0	= width0 <  2 * minWidth ?  2 * minWidth : width0;
		width1	= width1 < minWidth ? minWidth : width1;
		setColumnWidth (0, width0);
		setColumnWidth (1, width1);
		setColumnWidth (2, 3 * minWidth);
		// !v 2.29.1
		_mainSection	= section;
	}	// if (0 != section)
}	// QtPreferencesPanel::setRootSection


void QtPreferencesPanel::insertSection ( )
{
	CHECK_IF_EDITABLE ("Insertion de section impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question << "Saisissez le nom de la section à créer.";
	QString	name	= 
			QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de section invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		Section*	section	= 0 == item ? 
							0 : dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= 0 == item ?
							0 : dynamic_cast<NamedValue*>(&(item->getElement ( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		if (0 == parent)
		{
			parent	= _mainSection;
			item	= dynamic_cast<QtPrefsListViewItem*>(topLevelItem (0));
		}

		unique_ptr<Section>	newSection (
				new Section (QtUnicodeHelper::qstringToUTF8String (name)));
		if (0 != parent)
		{
			parent->addSection (newSection.get ( ));
			assert (0 != item);
			QtPrefsSectionListViewItem*	parent	=
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
			QtPrefsSectionListViewItem*	sectionItem	= 
				new QtPrefsSectionListViewItem (parent, newSection.release ( ), 
				                                item->isEditable ( ));
			sectionItem->setNameValidator (new QtXMLNameValidator (0));
			item->insertItem (sectionItem);
			setCurrentItem (sectionItem);
		}
		else
		{
			_mainSection	= newSection.release ( );
			QtPrefsSectionListViewItem*	sectionItem	=
				new QtPrefsSectionListViewItem (
									this, _mainSection, isEditable ( ));
			sectionItem->setNameValidator(new QtXMLNameValidator (0));
			insertTopLevelItem (0, sectionItem);
			setCurrentItem (sectionItem);
		}
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la section " << stdName
		         << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la section " << stdName
		         << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertSection


void QtPreferencesPanel::insertBooleanNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'une valeur bouléenne impossible : ")
	CHECK_SELECTION ("Insertion d'une valeur boulénne impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question << "Saisissez le nom de la valeur bouléenne à créer.";
	QString	name	= 
			QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de valeur booléenne invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<BoolNamedValue> newValue (
			new BoolNamedValue (QtUnicodeHelper::qstringToUTF8String (name)));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la valeur bouléenne " << stdName
		         << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la valeur bouléenne " << stdName
		         << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertBooleanNamedValue


void QtPreferencesPanel::insertColorNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'une couleur impossible : ")
	CHECK_SELECTION ("Insertion d'une couleur impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question << "Saisissez le nom de la couleur à créer.";
	QString	name	= 
			QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de couleur invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<ColorNamedValue>	newValue (
			new ColorNamedValue (QtUnicodeHelper::qstringToUTF8String (name),
			                     sectionItem->isEditable ( )));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la couleur " << stdName
		         << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la couleur " << stdName
		         << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertColorNamedValue


void QtPreferencesPanel::insertStringNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'une valeur de type chaine de caractères impossible : ")
	CHECK_SELECTION ("Insertion d'une valeur de type chaine de caractères impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question << "Saisissez le nom de la valeur de type chaine de caractères à créer.";
	QString	name	= 
			QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de valeur invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<StringNamedValue>	newValue (
		new StringNamedValue (QtUnicodeHelper::qstringToUTF8String (name)));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la valeur de type chaine de "
		         << "de caractères " << stdName
		         << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de la valeur de type chaine de "
		         << "caractères " << stdName << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertStringNamedValue


void QtPreferencesPanel::insertLongNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'un entier long impossible : ")
	CHECK_SELECTION ("Insertion d'un entier long impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question << "Saisissez le nom de l'entier long à créer.";
	QString	name	= 
		QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de valeur invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<LongNamedValue>	newValue (
			new LongNamedValue (QtUnicodeHelper::qstringToUTF8String (name)));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de l'entier long " << stdName
		         << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de l'entier long " << stdName
		         << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertLongNamedValue


void QtPreferencesPanel::insertUnsignedLongNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'un entier long non signé impossible : ")
	CHECK_SELECTION ("Insertion d'un entier long non signé impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question << "Saisissez le nom de l'entier long non signé à créer.";
	QString	name	= 
		QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de valeur invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<UnsignedLongNamedValue>	newValue (
					new UnsignedLongNamedValue (
						QtUnicodeHelper::qstringToUTF8String (name)));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de l'entier long non signé " 
		         << stdName << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible de l'entier long non signé " 
		         << stdName << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertUnsignedLongNamedValue


void QtPreferencesPanel::insertDoubleNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'un réel double précision impossible : ")
	CHECK_SELECTION("Insertion d'un réel double précision impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question <<"Saisissez le nom du réel double précision à créer.";
	QString	name	= 
		QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de valeur invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<DoubleNamedValue>	newValue (
						new DoubleNamedValue (
							QtUnicodeHelper::qstringToUTF8String (name)));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible du réel double précision " 
		         << stdName << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible du réel double précision " 
		         << stdName << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertDoubleNamedValue


void QtPreferencesPanel::insertDoubleTripletNamedValue ( )
{
	CHECK_IF_EDITABLE ("Insertion d'un triplet de réels double précision impossible : ")
	CHECK_SELECTION("Insertion d'un triplet de réels double précision impossible : ")
	bool			ok	= false;
	UTF8String		question (charset);
	question <<"Saisissez le nom du triplet de réels double précision à créer.";
	QString	name	= 
		QInputDialog::getText (this, _appTitle.c_str ( ), 
									UTF8TOQSTRING (question),
									QLineEdit::Normal, "", &ok);
	string	stdName	=name.toStdString ( );
	if (false == ok)
		return;

	try
	{
		QtXMLNameValidator	validator (0);
		int					pos	= 0;
		if (QValidator::Invalid == validator.validate (name, pos))
			throw Exception ("Nom de valeur invalide.");
		QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
		assert (0 != item);
		Section*	section	= dynamic_cast<Section*>(&(item->getElement ( )));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&(item->getElement( )));
		Section*	parent	= 0 == value ? section : &(value->getParent ( ));
		assert (0 != parent);
		QtPrefsSectionListViewItem*	sectionItem	= 
						dynamic_cast<QtPrefsSectionListViewItem*>(item);
		if (0 == sectionItem)
			sectionItem	= dynamic_cast<QtPrefsSectionListViewItem*>(item->parent ( ));
		assert (0 != sectionItem);

		unique_ptr<DoubleTripletNamedValue>	newValue (
					new DoubleTripletNamedValue (
							QtUnicodeHelper::qstringToUTF8String (name)));
		parent->addNamedValue (newValue.get ( ));
		QtPrefsNamedValueListViewItem*	valueItem	= 
			new QtPrefsNamedValueListViewItem (sectionItem, newValue.release( ),
			                                   sectionItem->isEditable ( ));
		valueItem->setNameValidator (new QtXMLNameValidator (0));
		sectionItem->insertItem (valueItem);
		setCurrentItem (valueItem);
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible du triplet de réels double "
		         << "précision " << stdName << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Insertion impossible du triplet de réels double "
		         << "précision " << stdName << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::insertDoubleTripletNamedValue


void QtPreferencesPanel::removeSelection ( )
{
	CHECK_IF_EDITABLE ("Suppression d'un élément impossible : ")
	CHECK_SELECTION ("Suppression d'un élément impossible : ")
	QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
	UnicodeString	name	= item->getElement ( ).getName ( );
	UTF8String		question (charset);
	question << "Souhaitez-vous réellement supprimer l'élément "
	         << name.ascii ( ) << " ?";
	if (0 != QMessageBox::information (this, _appTitle.c_str ( ),
					UTF8TOQSTRING (question), "Oui", "Non", QString ( ), 
					0, -1))
		return;

	try
	{
		Section*	section	= dynamic_cast<Section*>(&item->getElement ( ));
		NamedValue*	value	= dynamic_cast<NamedValue*>(&item->getElement ( ));

		if (true == item->getElement ( ).hasParent ( ))
		{
			if (0 != section)
				section->getParent ( ).removeSection (name);
			else if (0 != value)
				value->getParent ( ).removeNamedValue (name);
			else throw Exception ("Type d'élément incoonu.");
			delete item;
		}	// if (true == item->getElement ( ).hasParent ( ))
		else
		{
			Section*	rootSection	= releaseSection ( );
			delete rootSection;
		}	// else if (true == item->getElement ( ).hasParent ( ))
	}
	catch (const Exception& exc)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Suppression impossible de l'élément " 
		         << name.ascii ( ) << " :\n" << exc.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Suppression impossible de l'élément " 
		         << name.ascii ( ) << " :\nErreur non documentée.";
		displayErrorMessage (errorMsg);
	}
}	// QtPreferencesPanel::removeSelection


void QtPreferencesPanel::editComment ( )
{
	CHECK_SELECTION ("Consultation/Modification des commentaires d'un élément impossible : ")
	QtPrefsListViewItem*	item	=
					dynamic_cast<QtPrefsListViewItem*>(selectedItem ( ));
	assert (0 != item);
	Element*	element	= dynamic_cast<Element*>(&item->getElement ( ));
	assert (0 != element);

	UTF8String	title (_appTitle, charset);
	title << " : commentaires de " << element->getName ( );
	QtTextDialog	dialog (this, title, element->getComment( ), isEditable( ));
	if (QDialog::Accepted == dialog.exec ( ))
		item->setComment (dialog.getText ( ));
}	// QtPreferencesPanel::editComment


QTreeWidgetItem* QtPreferencesPanel::selectedItem ( )
{
	QList<QTreeWidgetItem*>	items	= selectedItems ( );
	if (true == items.isEmpty ( ))
		return 0;

	return items [0];
}	// QtPreferencesPanel::selectedItem


void QtPreferencesPanel::elementRenamed (QTreeWidgetItem* item, int col)
{
	QtPrefsListViewItem*			prefItem	= 
							dynamic_cast<QtPrefsListViewItem*>(item);
	QtPrefsNamedValueListViewItem*	nvItem	= 
							dynamic_cast<QtPrefsNamedValueListViewItem*>(item);
	if (0 == prefItem)
		return;

	try
	{
		if (false == prefItem->isEditable ( ))
		{
			UTF8String	errorMsg (charset);
			errorMsg << "Modification impossible de l'élément " 	
			         << prefItem->getElement ( ).getName ( )
			         << " :\nElément non éditable.";
			throw Exception (errorMsg);
		}	// if (false == prefItem->isEditable ( ))
		if (0 == col)
			prefItem->nameModified ( ); 
		else if ((1 == col) && (0 != nvItem))
			nvItem->valueModified ( );
		else if (2 == col)
			prefItem->commentModified ( );
	}
	catch (const Exception& exc)
	{
		displayErrorMessage (exc.getMessage ( ));
		prefItem->setText (
					0, UTF8TOQSTRING (prefItem->getElement ( ).getName ( )));
		if (0 != nvItem)
			nvItem->setText (1, 
						nvItem->getNamedValue ( ).getStrValue ( ).c_str ( ));
	}
	catch (...)
	{
		UTF8String	errorMsg (charset);
		if (0 == col)
			errorMsg << "Impossibilité de renommer l'élément "
			         << prefItem->getElement ( ).getName ( ) << " en "
			         << QtUnicodeHelper::qstringToUTF8String(prefItem->text (0))
			         << " :\n" << "Erreur non documentée.";
		else
		{
			assert (0 != nvItem);
			errorMsg << "Impossibilité d'affecter la valeur "
			         << QtUnicodeHelper::qstringToUTF8String(nvItem->text (1))
			         << " à l'élément "
			         << nvItem->getElement ( ).getName ( ) << " :\n"
			         << "Erreur non documentée.";
		}
		displayErrorMessage (errorMsg.iso ( ));
		if (0 == col)
			prefItem->setText (0, UTF8TOQSTRING (
								prefItem->getElement( ).getName( )));
		else
		{
			assert (0 != nvItem);
			nvItem->setText (1, 
						nvItem->getNamedValue ( ).getStrValue ( ).c_str ( ));
		}
	}
}	// QtPreferencesPanel::elementRenamed


void QtPreferencesPanel::displayErrorMessage (const UTF8String& message)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));
	QtMessageBox::displayErrorMessage (this, _appTitle, message);
	QApplication::restoreOverrideCursor ( );
}	// QtPreferencesPanel::displayErrorMessage


