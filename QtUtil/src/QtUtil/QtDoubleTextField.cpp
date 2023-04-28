#include "QtUtil/QtDoubleTextField.h"
#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/InternalError.h>
#include <TkUtil/NumericConversions.h>
#include <TkUtil/NumericServices.h>

#include <cmath>
#include <assert.h>



USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtDoubleTextField::QtDoubleTextField (
		QWidget* parent, bool autoValidation, const char* name)
	: QtValidatedTextField (parent, autoValidation, name)
{
	createGui ( );
}	// QtDoubleTextField::QtDoubleTextField


QtDoubleTextField::QtDoubleTextField (
		double value, QWidget* parent, bool autoValidation, const char* name)
	: QtValidatedTextField (parent, autoValidation, name)
{
	createGui ( );
	setValue (value);
}	// QtDoubleTextField::QtDoubleTextField


QtDoubleTextField::QtDoubleTextField (const QtDoubleTextField&)
	: QtValidatedTextField (0, 0)
{
	assert (0 && "QtDoubleTextField::QtDoubleTextField is not allowed.");
}	// QtDoubleTextField::QtDoubleTextField (const QtDoubleTextField&)


QtDoubleTextField& QtDoubleTextField::operator = (const QtDoubleTextField&)
{
	assert (0 && "QtDoubleTextField::operator = is not allowed.");
	return *this;
}	// QtDoubleTextField::operator =


QtDoubleTextField::~QtDoubleTextField ( )
{
}	// QtDoubleTextField::~QtDoubleTextField


bool QtDoubleTextField::isNumeric ( ) const
{
	return true;
}	// QtDoubleTextField::isNumeric


double QtDoubleTextField::getValue ( ) const
{
	bool				ok				= true;
	QString				tValue			= text ( );
	int					pos				= 0;
	double				dValue			= tValue.toDouble (&ok);
	const QDoubleValidator&	validator	= getValidator ( );

	// V 2.44.1 :
	if (false == ok)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Valeur " << tValue.toStdString ( ) << " incorrecte.\n";
		throw Exception (errorMsg);
	}	// if (false == ok)
	if ((dValue < validator.bottom ( )) || (dValue > validator.top ( )))
	{
		if ((fabs (dValue - validator.bottom ( )) >
								NumericServices::doubleMachEpsilon ( )) &&
		    (fabs (dValue - validator.top ( )) >
								NumericServices::doubleMachEpsilon ( )))
		{
			UTF8String	errorMsg (charset);
			errorMsg << "Valeur " << tValue.toStdString ( ) << " incorrecte.\n";
			errorMsg << "La valeur doit être comprise entre "
			         << NumericConversions::userRepresentation ( // v 2.48.0
							validator.bottom ( ),
							NumericServices::doubleScientificNotationCharMax, 1)
			         << " et " << NumericConversions::userRepresentation (
							validator.top ( ),
							NumericServices::doubleScientificNotationCharMax, 1)
					 << ".";
//			         << ios_base::fixed << IN_UTIL setprecision (2) // v 2.48.0
//			         << validator.bottom ( )
//			         << " et " << validator.top ( ) << ".";
			throw Exception (errorMsg);
		}	// if ((dValue < validator.bottom ( )) || ...
	}	// if ((dValue < validator.bottom ( )) || (dValue > validator.top ( )))
	// FIN V 2.44.1
/* V <= 2.44.0
	if ((false == ok) ||
	    (QValidator::Acceptable != validator.validate (tValue, pos)))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Valeur " << tValue.toStdString ( ) << " incorrecte.\n";

		if ((validator.bottom ( ) == -NumericServices::doubleMachMax ( )) &&
		    (validator.top ( ) == NumericServices::doubleMachMax ( )))
			errorMsg << "La valeur doit être de type réel double précision.";
		else if ((validator.bottom ( ) == -NumericServices::floatMachMax ( )) &&
		    (validator.top ( ) == NumericServices::floatMachMax ( )))
			errorMsg << "La valeur doit être de type réel simple précision.";
		else
			errorMsg << "La valeur doit être comprise entre "
		         << ios_base::fixed << IN_UTIL setprecision (2)
		         << validator.bottom ( )
		         << " et " << validator.top ( ) << ".";
		throw Exception (errorMsg);
	}	// if ((false == ok)
*/

	return dValue;
}	// QtDoubleTextField::getValue


void QtDoubleTextField::setValue (double value)
{
	const QDoubleValidator&	validator	= getValidator ( );
	if ((value < validator.bottom ( )) || (value > validator.top ( )))
	{
		UTF8String errorMsg (charset);
		errorMsg << "Erreur : la valeur " << value
		         << " est en dehors du domaine autorisé ("
			         << NumericConversions::userRepresentation ( // v 2.48.0
							validator.bottom ( ),
							NumericServices::doubleScientificNotationCharMax, 1)
					<< " - "
			         << NumericConversions::userRepresentation ( // v 2.48.0
							validator.top ( ),
							NumericServices::doubleScientificNotationCharMax, 1)
//		         << ios_base::fixed << IN_UTIL setprecision (2)
//		         << validator.bottom ( ) << " - " << validator.top ( )
		         << ").";
		throw Exception (errorMsg);
	}	// if ((value < validator.bottom ( )) || (value > validator.top ( )))

	// On évite de perdre des epsilons avec le format d'affichage par défaut des
	// streams c++ :
//	setText (UTF8TOQSTRING (value));
	UTF8String	us;
	us << (QDoubleValidator::StandardNotation == getNotation ( ) ?
									ios_base::fixed : ios_base::scientific);
	us << IN_UTIL setprecision (validator.decimals ( )) << value;
	setText (UTF8TOQSTRING (us));
	getValue ( );
}	// QtDoubleTextField::setValue


void QtDoubleTextField::getRange (double& min, double& max) const
{
	const QDoubleValidator&	validator	= getValidator ( );
	min	= validator.bottom ( );
	max	= validator.top ( );
}	// QtDoubleTextField::getRange


void QtDoubleTextField::setRange (double min, double max)
{
	if (min >= max)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Domaine de saisie d'un réel invalide : ("
		         << NumericConversions::userRepresentation (min,
						NumericServices::doubleScientificNotationCharMax, 1)
		         << " - "
		         << NumericConversions::userRepresentation (max,
						NumericServices::doubleScientificNotationCharMax, 1)
//		         << ios_base::fixed << IN_UTIL setprecision (2)
//		         << min << " - " << max
		         << ").";
		throw Exception (errorMsg);
	}	// if (min >= max)

	const QDoubleValidator&	old	= getValidator ( );
	QDoubleValidator*		v	= cloneValidator ( );
	v->setRange (min, max, getDecimals ( ));
	setValidator (v);
}	// QtDoubleTextField::setRange


unsigned short QtDoubleTextField::getDecimals ( ) const
{
	return (unsigned short)getValidator ( ).decimals ( );
}	// QtDoubleTextField::getDecimals


void QtDoubleTextField::setDecimals (unsigned short num)
{
	const QDoubleValidator&	old	= getValidator ( );
	QDoubleValidator*		v	= cloneValidator ( );
	v->setDecimals (num);
	setValidator (v);
}	// QtDoubleTextField::setDecimals


QDoubleValidator::Notation QtDoubleTextField::getNotation (  ) const
{
	return getValidator ( ).notation ( );
}	// QtDoubleTextField::getNotation


void QtDoubleTextField::setNotation (QDoubleValidator::Notation notation)
{
	const QDoubleValidator&	old	= getValidator ( );
	if (notation == old.notation ( ))
		return;

	double	value	= 0.;
	bool	valid	= false;
	try
	{
		value	= getValue ( );
		valid	= true;
	}
	catch (...)
	{
	}
	
	QDoubleValidator*		v	= cloneValidator ( );
	v->setNotation (notation);
	setValidator (v);

	try
	{
		if (true == valid)
			setValue (value);
	}
	catch (...)
	{
	}
}	// QtDoubleTextField::setNotation


void QtDoubleTextField::createGui ( )
{
	QDoubleValidator*	validator	= new QDoubleValidator (this);
	setValidator (validator);
	setDecimals (8);
}	// QtDoubleTextField::createGui


const QDoubleValidator& QtDoubleTextField::getValidator ( ) const
{
	const QDoubleValidator*	v= 
						dynamic_cast<const QDoubleValidator*>(validator ( ));
	if (0 == v)
	{
		INTERNAL_ERROR (exc, "Absence de validateur.", "QtDoubleTextField::getValidator")
		throw exc;
	}	// if (0 == v)

	return *v;
}	// QtDoubleTextField::getValidator


QDoubleValidator* QtDoubleTextField::cloneValidator ( )
{
	const QDoubleValidator&	old	= getValidator ( );
	QDoubleValidator*		v	=
		new QDoubleValidator (old.bottom( ), old.top( ), old.decimals( ), this);
	v->setNotation (old.notation ( ));

	return v;
}	// QtDoubleTextField::cloneValidator


bool QtDoubleTextField::validate ( )
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
}	// QtDoubleTextField::validate


