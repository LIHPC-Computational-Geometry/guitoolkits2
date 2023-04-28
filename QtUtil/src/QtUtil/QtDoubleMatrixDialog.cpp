#include "QtUtil/QtDoubleMatrixDialog.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/Exception.h>
#include <TkUtil/MemoryError.h>
#include <TkUtil/UTF8String.h>

#include <string.h>
#include <assert.h>

USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtDoubleMatrixDialog::QtDoubleMatrixDialog (
		QWidget* parent, const UTF8String& title,
		size_t rows, size_t columns, const double* const* matrix,
		const string& helpURL, const string& helpTag)
	: QtMatrixDialog (parent, title, rows, columns, helpURL, helpTag),
	  _matrix (0), _validator (0)
{
	_matrix	= new double* [rows];
	CHECK_NULL_PTR_ERROR (_matrix)
	for (size_t i = 0; i < rows; i++)
	{
		_matrix [i]	= new double [columns];
		CHECK_NULL_PTR_ERROR (_matrix [i])
		memcpy (_matrix [i], matrix [i], columns * sizeof (double));

		for (size_t j = 0; j < columns; j++)
		{
			QLineEdit&		editor	= getEditor (i, j);
			UTF8String	value (charset);
			// setprecision : ne pas perdre en precision
			// fixed : ne pas avoir un exposant
			// substring : on enleve tous les 0 à la fin. Sinon, 1 sera 
			// affiché 1.0000000000000...
			value << IN_UTIL setprecision(15) << ios_base::fixed
			      << _matrix [i][j];
			size_t	k	= value.length ( ) - 1;
			if (((size_t)-1 !=  value.find ('.')) &&
			    ((size_t)-1 == value.find ('e'))   &&
			    ((size_t)-1 == value.find ('E')))
				for (; (k > 0) && ('0' == *value [k]); k--);	// UTF8String
//				for (; (k > 0) && ('0' == value [k]); k--);	// UnicodeString
			value	= value.substring (0, k);
			if ((0 == i) && (0 == j))
				_validator	= new QDoubleValidator (&editor);
			editor.setValidator (_validator);
			editor.setText (UTF8TOQSTRING (value));
		}	// for (size_t j = 0; j < columns; j++)
	}	// for (size_t i = 0; i < rows; i++)
}	// QtDoubleMatrixDialog::QtDoubleMatrixDialog


QtDoubleMatrixDialog::QtDoubleMatrixDialog (const QtDoubleMatrixDialog& dlg)
	: QtMatrixDialog (0, "", 0, 0),
	  _matrix (0), _validator (0)
{
    assert (0 && "QtDoubleMatrixDialog copy constructor is forbidden.");
}   // QtDoubleMatrixDialog::QtDoubleMatrixDialog (const QtDoubleMatrixDialog&)

    
QtDoubleMatrixDialog& QtDoubleMatrixDialog::operator = (
												const QtDoubleMatrixDialog&)
{
    assert (0 && "QtDoubleMatrixDialog assignment operator is forbidden.");
    return *this;
}   // QtDoubleMatrixDialog::operator =
    
    
QtDoubleMatrixDialog::~QtDoubleMatrixDialog ( )
{
	for (size_t i = 0; i < getRowsNum ( ); i++)
		delete [] _matrix [i];
	delete [] _matrix;		_matrix	= 0;
}   // QtDoubleMatrixDialog::~QtDoubleMatrixDialog


const double* const* QtDoubleMatrixDialog::getMatrix ( ) const
{
	for (size_t i = 0; i < getRowsNum ( ); i++)
		for (size_t j = 0; j < getColumnsNum ( ); j++)
			_matrix [i][j]	= getValue (i, j);
	return _matrix;
}	// QtDoubleMatrixDialog::getMatrix


void QtDoubleMatrixDialog::getMatrix (double*const*const matrix) const
{
	for (size_t i = 0; i < getRowsNum ( ); i++)
		for (size_t j = 0; j < getColumnsNum ( ); j++)
			matrix [i][j]	= getValue (i, j);
}	// QtDoubleMatrixDialog::getMatrix


double QtDoubleMatrixDialog::getValue (size_t i, size_t j) const
{
	QString				strValue	= getStringValue (i, j);
	bool				ok			= true;
	int					pos			= 0;
	double				value		= strValue.toDouble (&ok);
	QDoubleValidator&	validator	= getValidator ( );

	if ((false == ok) ||
			(QValidator::Acceptable != validator.validate (strValue, pos)))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Valeur (" << (unsigned long)i << ", " << (unsigned long)j
		         << ") de la matrice.";
		errorMsg << "\n" << "La valeur doit être comprise entre "
		         << ios_base::fixed << IN_UTIL setprecision (2)
		         << validator.bottom ( ) << " et " << validator.top ( )
		         <<".";
		throw Exception (errorMsg);
	}	// if ((false == ok) || ((0 != _validator) && ...

	return value;
}	// QtDoubleMatrixDialog::getValue


QDoubleValidator& QtDoubleMatrixDialog::getValidator ( ) const
{
	CHECK_NULL_PTR_ERROR (_validator)
	return *_validator;
}	// QtDoubleMatrixDialog::getValidator


