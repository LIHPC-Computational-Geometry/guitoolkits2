#include "QtUtil/QtStringListSelectionPanel.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"

#ifndef QT_5
#include <QtGui/QHeaderView>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWhatsThis>
#else	// QT_5
#include <QHeaderView>
#include <QLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWhatsThis>
#endif	// QT_5

#include <assert.h>
#include <iostream>

static const TkUtil::Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION

USING_STD


QtStringListSelectionPanel::QtStringListSelectionPanel (
		QWidget* parent, const vector <string>& unselected, 
		const vector <string>& selected)
	: QtGroupBox ("", parent),
	  _unselectedListView (0), _selectedListView (0),
	  _addButton (0), _removeButton (0)
{
	QHBoxLayout*	layout	= new QHBoxLayout (this);
	setLayout (layout);
	setMargin (QtConfiguration::margin);
	setSpacing (QtConfiguration::spacing);

	// Liste des chaines non selectionnees :
	_unselectedListView	= new QListWidget (this);
	_unselectedListView->setModelColumn (1);
	_unselectedListView->setSelectionMode (QAbstractItemView::MultiSelection);
	connect (_unselectedListView, 
		SIGNAL(itemDoubleClicked (QListWidgetItem*)), this,
		SLOT (addStringCallback (QListWidgetItem*)));
	layout->addWidget (_unselectedListView);
	_unselectedListView->setWhatsThis (
	                 "Liste des noms non sélectionnés.");
	for (vector<string>::const_iterator itu = unselected.begin ( );
	     unselected.end ( ) != itu; itu++)
	{
		QListWidgetItem*	item	= 
				new QListWidgetItem ((*itu).c_str ( ), _unselectedListView);
		_unselectedListView->addItem (item);
	}

	// Les boutons :
	QWidget*		arrowsBox	= new QWidget (this);
	QVBoxLayout*	arrowsLayout	= new QVBoxLayout ( );
	arrowsBox->setLayout (arrowsLayout);
	layout->addWidget (arrowsBox);
	QLabel*	label1	= new QLabel (" ", arrowsBox);
	_addButton	= new QPushButton (QSTR ("Sélectionner ->"), arrowsBox);
	connect (_addButton, SIGNAL(clicked ( )), this, 
	         SLOT (addStringsCallback ( )));
	QLabel*	label2	= new QLabel (" ", arrowsBox);
	_removeButton	= new QPushButton (QSTR ("<- Désélectionner"), arrowsBox);
	connect (_removeButton, SIGNAL(clicked ( )), this,
	         SLOT (removeStringsCallback ( )));
	QLabel*	label3	= new QLabel (" ", arrowsBox);
	_addButton->setFixedSize (_removeButton->sizeHint ( ));
	_removeButton->setFixedSize (_removeButton->sizeHint ( ));
	_addButton->setWhatsThis ("Ajoute les noms sélectionnés à la sélection.");
	arrowsLayout->addWidget (label1);
	arrowsLayout->addWidget (_addButton);
	arrowsLayout->addWidget (label2);
	arrowsLayout->addWidget (_removeButton);
	arrowsLayout->addWidget (label3);

	// Les chaines selectionnees :
	_selectedListView	= new QListWidget (this);
	_selectedListView->setModelColumn (1);
	_selectedListView->setSelectionMode (QAbstractItemView::MultiSelection);
	_selectedListView->setWhatsThis ("Liste des noms sélectionnés.");
	connect (_selectedListView, 
		SIGNAL(itemDoubleClicked (QListWidgetItem*)), this,
		SLOT (removeStringCallback (QListWidgetItem*)));
	layout->addWidget (_selectedListView);
	for (vector<string>::const_iterator its = selected.begin ( );
	     selected.end ( ) != its; its++)
	{
		QListWidgetItem*	item	= 
				new QListWidgetItem ((*its).c_str ( ), _selectedListView);
		_selectedListView->addItem (item);
	}

	QSize	unselectedSize	= _unselectedListView->sizeHint ( );
	QSize	selectedSize	= _selectedListView->sizeHint ( );
	QSize	buttonSize		= _removeButton->sizeHint ( );
	QSize	size;
	size.setWidth (unselectedSize.width ( ) < selectedSize.width ( ) ?
	               selectedSize.width ( ) : unselectedSize.width ( ));
	if (size.width ( ) < (2 * buttonSize.width ( )))
		size.setWidth (2 * buttonSize.width ( ));
	size.setHeight (unselectedSize.height ( ) < selectedSize.height ( ) ?
	                selectedSize.height ( ) : unselectedSize.height ( ));
	_selectedListView->setMinimumSize (size);
	_unselectedListView->setMinimumSize (size);
}	// QtStringListSelectionPanel::QtStringListSelectionPanel


QtStringListSelectionPanel::QtStringListSelectionPanel (
			const QtStringListSelectionPanel&)
	: QtGroupBox ("", 0), _unselectedListView (0), _selectedListView (0),
	  _addButton (0), _removeButton (0)
{
	assert (0 && "QtStringListSelectionPanel::QtStringListSelectionPanel is not alllowed.");
}	// QtStringListSelectionPanel::QtStringListSelectionPanel


QtStringListSelectionPanel& QtStringListSelectionPanel::operator = (
		const QtStringListSelectionPanel&)
{
	assert (0 && "QtStringListSelectionPanel::operator = is not alllowed.");
	return *this;
}	// QtStringListSelectionPanel::operator =


QtStringListSelectionPanel::~QtStringListSelectionPanel ( )
{
}	// QtStringListSelectionPanel::~QtStringListSelectionPanel


vector<string> QtStringListSelectionPanel::getSelection ( ) const
{
	assert ((0 != _selectedListView) && "QtStringListSelectionPanel::getSelection : null selected string list view.");
	vector<string>	strings;

	for (int i = 0; i < _selectedListView->count ( ); i++)
		strings.push_back (_selectedListView->item (i)->text ( ).toStdString ( ));

	return strings;
}	// QtStringListSelectionPanel::getSelection


void QtStringListSelectionPanel::addStringsCallback ( )
{
	assert ((0 != _selectedListView) && "QtStringListSelectionPanel::addStringsCallback : null selected string list view.");
	assert ((0 != _unselectedListView) && "QtStringListSelectionPanel::addStringsCallback : null unselected string list view.");

	QList<QListWidgetItem*>	selection	= _unselectedListView->selectedItems( );
	int						i	= 0;
	for (QList<QListWidgetItem*>::iterator it = selection.begin ( );
	     it != selection.end ( ); it++, i++)
	{
		QListWidgetItem*	item	= *it;
		_selectedListView->addItem (
					new QListWidgetItem (item->text ( ), _selectedListView));
		_unselectedListView->takeItem (_unselectedListView->row (item));
		delete item;
	}	// for (QList<QListWidgetItem*>::iterator it = selection.begin ( ); ...

	assert ((0 != _addButton) && "QtStringListSelectionPanel::addStringsCallback : null add button.");
	assert ((0 != _removeButton) && "QtStringListSelectionPanel::addStringsCallback : null remove button.");
	_addButton->setEnabled (0==_unselectedListView->count( ) ?  false : true);
	_removeButton->setEnabled (0==_selectedListView->count( ) ?  false : true);
}	// QtStringListSelectionPanel::addStringsCallback


void QtStringListSelectionPanel::removeStringsCallback ( )
{
	assert ((0 != _selectedListView) && "QtStringListSelectionPanel::removeStringsCallback : null selected string list view.");
	assert ((0 != _unselectedListView) && "QtStringListSelectionPanel::removeStringsCallback : null unselected string list view.");

	QList<QListWidgetItem*>	selection	= _selectedListView->selectedItems( );
	int						i	= 0;
	for (QList<QListWidgetItem*>::iterator it = selection.begin ( );
	     it != selection.end ( ); it++, i++)
	{
		QListWidgetItem*	item	= *it;
		_unselectedListView->addItem (
					new QListWidgetItem (item->text ( ), _unselectedListView));
		_selectedListView->takeItem (_selectedListView->row (item));
		delete item;
	}	// for (QList<QListWidgetItem*>::iterator it = selection.begin ( ); ...

	assert ((0 != _addButton) && "QtStringListSelectionPanel::removeStringsCallback : null add button.");
	assert ((0 != _removeButton) && "QtStringListSelectionPanel::removeStringsCallback : null remove button.");
	_addButton->setEnabled (0==_unselectedListView->count( ) ?  false : true);
	_removeButton->setEnabled (0==_selectedListView->count( ) ?  false : true);
}	// QtStringListSelectionPanel::removeStringsCallback


void QtStringListSelectionPanel::addStringCallback (
								QListWidgetItem* item)
{
	assert ((0 != _selectedListView) && "QtStringListSelectionPanel::addStringCallback : null selected string list view.");
	assert ((0 != _unselectedListView) && "QtStringListSelectionPanel::addStringCallback : null unselected string list view.");
	if (0 == item)
		return;

	_selectedListView->addItem (
					new QListWidgetItem (item->text ( ), _selectedListView));
	_unselectedListView->takeItem (_unselectedListView->row (item));
	delete item;	item	= 0;

	assert ((0 != _addButton) && "QtStringListSelectionPanel::addStringCallback : null add button.");
	assert ((0 != _removeButton) && "QtStringListSelectionPanel::addStringCallback : null remove button.");
	_addButton->setEnabled (0==_unselectedListView->count( ) ?  false : true);
	_removeButton->setEnabled (0==_selectedListView->count( ) ?  false : true);
}	// QtStringListSelectionPanel::addStringCallback


void QtStringListSelectionPanel::removeStringCallback (
								QListWidgetItem* item)
{
	assert ((0 != _selectedListView) && "QtStringListSelectionPanel::removeStringCallback : null selected string list view.");
	assert ((0 != _unselectedListView) && "QtStringListSelectionPanel::removeStringCallback : null unselected string list view.");
	if (0 == item)
		return;

	_unselectedListView->addItem (
					new QListWidgetItem (item->text ( ), _unselectedListView));
	_selectedListView->takeItem (_selectedListView->row (item));

	assert ((0 != _addButton) && "QtStringListSelectionPanel::removeStringCallback : null add button.");
	assert ((0 != _removeButton) && "QtStringListSelectionPanel::removeStringCallback : null remove button.");
	_addButton->setEnabled (0==_unselectedListView->count( ) ?  false : true);
	_removeButton->setEnabled (0==_selectedListView->count( ) ?  false : true);
}	// QtStringListSelectionPanel::removeStringCallback




