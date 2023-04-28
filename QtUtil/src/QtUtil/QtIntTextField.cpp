#include "QtUtil/QtIntTextField.h"
#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/InternalError.h>
#include <TkUtil/NumericServices.h>

#include <QValidator>

#include <assert.h>



USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtIntTextField::QtIntTextField (
						QWidget* parent, bool autoValidation, const char* name)
	: QtValidatedTextField (parent, autoValidation, name)
{
	createGui ( );
}	// QtIntTextField::QtIntTextField


QtIntTextField::QtIntTextField (
			int value, QWidget* parent, bool autoValidation, const char* name)
	: QtValidatedTextField (parent, autoValidation, name)
{
	createGui ( );
	setValue (value);
}	// QtIntTextField::QtIntTextField


QtIntTextField::QtIntTextField (const QtIntTextField&)
	: QtValidatedTextField (0, true, 0)
{
	assert (0 && "QtIntTextField::QtIntTextField is not allowed.");
}	// QtIntTextField::QtIntTextField (const QtIntTextField&)


QtIntTextField& QtIntTextField::operator = (const QtIntTextField&)
{
	assert (0 && "QtIntTextField::operator = is not allowed.");
	return *this;
}	// QtIntTextField::operator =


QtIntTextField::~QtIntTextField ( )
{
}	// QtIntTextField::~QtIntTextField


bool QtIntTextField::isNumeric ( ) const
{
	return true;
}	// QtIntTextField::isNumeric


int QtIntTextField::getValue ( ) const
{
	bool					ok			= true;
	QString					tValue		= text ( );
	int						pos			= 0;
	int						iValue		= tValue.toInt (&ok);
	const QIntValidator&	validator	= getValidator ( );

	if ((false == ok) ||
	    (QValidator::Acceptable != validator.validate (tValue, pos)))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Valeur " << tValue.toStdString ( ) << " incorrecte.\n";

		if ((validator.bottom ( ) == -NumericServices::longMachMin ( )) &&
		    (validator.top ( ) == NumericServices::longMachMin ( )))
			errorMsg << "La valeur doit être de type entier long.";
		else if ((validator.bottom ( ) == NumericServices::unsignedLongMachMin ( )) &&
		    (validator.top ( ) == NumericServices::unsignedLongMachMax ( )))
			errorMsg << "La valeur doit être de type entier long non signé.";
		else if ((validator.bottom ( ) == -NumericServices::shortMachMin ( )) &&
		    (validator.top ( ) == NumericServices::shortMachMin ( )))
			errorMsg << "La valeur doit être de type entier court (short).";
		else if ((validator.bottom ( ) == NumericServices::unsignedShortMachMin ( )) &&
		    (validator.top ( ) == NumericServices::unsignedShortMachMax ( )))
			errorMsg << "La valeur doit être de type entier court non signé.";
		else if ((validator.bottom ( ) == -NumericServices::charMachMin ( )) &&
		    (validator.top ( ) == NumericServices::charMachMin ( )))
			errorMsg << "La valeur doit être de type caractère.";
		else if ((validator.bottom ( ) == NumericServices::unsignedCharMachMin ( )) &&
		    (validator.top ( ) == NumericServices::unsignedCharMachMax ( )))
			errorMsg << "La valeur doit être de type caractère non signé.";
		else
			errorMsg << "La valeur doit être comprise entre "
		         << ios_base::fixed << IN_UTIL setprecision (2)
		         << (long)validator.bottom ( ) << " et "
		         << (long)validator.top ( ) << ".";
		throw Exception (errorMsg);
	}	// if ((false == ok)

	return iValue;
}	// QtIntTextField::getValue


void QtIntTextField::setValue (int value)
{
	const QIntValidator&	validator	= getValidator ( );
	if ((value < validator.bottom ( )) || (value > validator.top ( )))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Erreur : la valeur " << (long)value
		         << " est en dehors du domaine autorisé ("
		         << (long)validator.bottom ( ) << " - "
		         << (long)validator.top ( ) << ").";
		throw Exception (errorMsg);
	}	// if ((value < validator.bottom ( )) || (value > validator.top ( )))

	setText (UTF8TOQSTRING (UTF8String ((long)value)));
	getValue ( );
}	// QtIntTextField::setValue


void QtIntTextField::getRange (int& min, int& max) const
{
	const QIntValidator&	validator	= getValidator ( );
	min	= validator.bottom ( );
	max	= validator.top ( );
}	// void QtIntTextField::getRange


void QtIntTextField::setRange (int min, int max)
{
	if (min >= max)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Domaine de saisie d'un réel invalide : ("
		         << (long)min << " - " << (long)max << ").";
		throw Exception (errorMsg);
	}	// if (min >= max)

	const QIntValidator&	old	= getValidator ( );
	QIntValidator*			v	= new QIntValidator (min, max, this);
	setValidator (v);
}	// QtIntTextField::setRange


void QtIntTextField::createGui ( )
{
	QIntValidator*	validator	= new QIntValidator (this);
	setValidator (validator);
}	// QtIntTextField::createGui


const QIntValidator& QtIntTextField::getValidator ( ) const
{
	const QIntValidator*	v= 
						dynamic_cast<const QIntValidator*>(validator ( ));
	if (0 == v)
	{
		INTERNAL_ERROR (exc, "Absence de validateur.", "QtIntTextField::getValidator")
		throw exc;
	}	// if (0 == v)

	return *v;
}	// QtIntTextField::getValidator


bool QtIntTextField::validate ( )
{
	try
	{
		getValue ( );
		setSkin (true);
	}
	catch (const Exception& exc)
	{
		setSkin (false);
		if (true == QtValidatedTextField::dialogOnError)
			QtMessageBox::displayErrorMessage (
							this, "Saisie invalide", exc.getFullMessage ( ));
		return false;
	}
	catch (...)
	{
		setSkin (false);
		if (true == QtValidatedTextField::dialogOnError)
			QtMessageBox::displayErrorMessage (
							this, "Saisie invalide", "Erreur non documentée.");
		return false;
	}

	return true;
}	// QtIntTextField::validate


