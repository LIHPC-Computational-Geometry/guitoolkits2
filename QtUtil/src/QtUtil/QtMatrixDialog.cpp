#include "QtUtil/QtMatrixDialog.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/QtGroupBox.h"

#include <TkUtil/Exception.h>
#include <TkUtil/MemoryError.h>

#ifndef QT_5
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#else	// QT_5
#include <QLabel>
#include <QLayout>
#endif	// QT_5

#include <assert.h>



USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtMatrixDialog::QtMatrixDialog (
	QWidget* parent, const UTF8String& title, size_t rows, size_t columns,
	const string& helpURL, const string& helpTag)
	: QDialog (parent, QtConfiguration::modalDialogWFlags),
	  _valuesTextField (0), _closurePanel (0), _rows (rows), _columns (columns)
{	
	setModal (true);
	setWindowTitle (UTF8TOQSTRING (title));

	// Creation de l'ihm :
	QVBoxLayout*	vLayout	= new QVBoxLayout (this);
	vLayout->setMargin (QtConfiguration::margin);
	vLayout->setSizeConstraint (QLayout::SetMinimumSize);

	// Le panneau :
	QtGroupBox*	panel	= new QtGroupBox ("Matrice de transformation", this);
	vLayout->addWidget (panel);
	QGridLayout*	layout	= new QGridLayout (panel);
	layout->setMargin (QtConfiguration::margin);
	layout->setSpacing (QtConfiguration::spacing);
	layout->setSizeConstraint (QLayout::SetMinimumSize);
	panel->setLayout (layout);
	if ((0 != _rows) && (0 != _columns))
	{
		size_t	i			= 0, j = 0;
		_valuesTextField	= new QLineEdit** [_rows];
		CHECK_NULL_PTR_ERROR (_valuesTextField)
		for (i = 0; i < _columns; i++)
		{
			_valuesTextField [i]	= new QLineEdit* [_columns];
			CHECK_NULL_PTR_ERROR (_valuesTextField [i])

			for (j = 0; j < _columns; j++)
			{
				_valuesTextField [i][j]	= new QLineEdit (panel);	
				layout->addWidget (_valuesTextField [i][j], i, j);
			}	// for (j = 0; j < _columns; j++)
		}
	}	// if ((0 != _rows) && (0 != _columns))
	
	// Les boutons :
	vLayout->addWidget (new QLabel (" ", this));
	_closurePanel	= 
		new QtDlgClosurePanel (this, false, "Appliquer", "", "Annuler", helpURL,
		                       helpTag);
	vLayout->addWidget (_closurePanel);
	_closurePanel->setMinimumSize (_closurePanel->sizeHint ( ));
	connect (_closurePanel->getApplyButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(accept ( )));
	connect (_closurePanel->getCancelButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(reject ( )));

	// Par defaut le bouton OK est artificellement clique par QDialog quand
	// l'utilisateur fait return dans un champ de texte => on inhibe ce
	// comportement par defaut :
	_closurePanel->getApplyButton ( )->setAutoDefault (false);
	_closurePanel->getApplyButton ( )->setDefault (false);
	_closurePanel->getCancelButton ( )->setAutoDefault (false);
	_closurePanel->getCancelButton ( )->setDefault (false);

	vLayout->activate ( );
	setMinimumSize (vLayout->sizeHint ( ));
}	// QtMatrixDialog::QtMatrixDialog


QtMatrixDialog::QtMatrixDialog (const QtMatrixDialog&)
{
	assert (0 && "QtMatrixDialog copy constructor is forbidden.");
}	// QtMatrixDialog::QtMatrixDialog (const QtMatrixDialog&)


QtMatrixDialog& QtMatrixDialog::operator = (const QtMatrixDialog&)
{
	assert (0 && "QtMatrixDialog assignment operator is forbidden.");
	return *this;
}	// QtMatrixDialog::operator =


QtMatrixDialog::~QtMatrixDialog ( )
{
}	// QtMatrixDialog::~QtMatrixDialog


QString QtMatrixDialog::getStringValue (size_t i, size_t j) const
{
	return getEditor (i, j).text ( );
}	// QtMatrixDialog::getStringValue 


QLineEdit& QtMatrixDialog::getEditor (size_t i, size_t j) const
{
	if ((i >= _rows) || (j >= _columns))
	{
		UTF8String	message (charset);
		message << "Indice de cellule dans une matrice ("
		        << (unsigned long)_rows << ", " << (unsigned long)_columns
		        << ") invalide : (" << (unsigned long)i << ", "
		        << (unsigned long)j << ").\n" 
		        << "Erreur relevée dans QtMatrixDialog::getEditor.";
		throw Exception (message);
	}	// if ((i >= _rows) || (j >= _columns))

	assert (0 != _valuesTextField);
	return *_valuesTextField [i][j];
}	// QtMatrixDialog::getEditor
