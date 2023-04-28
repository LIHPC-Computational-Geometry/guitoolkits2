/**
 * @file        QtCoordinatesTextField.cpp
 * @author      Charles PIGNEROL
 * @date        15/03/2017
 */

#include "QtUtil/QtCoordinatesTextField.h"
#include "QtUtil/QtUnicodeHelper.h"

#include <QtUtil/QtMessageBox.h>
#include <QtUtil/QtObjectSignalBlocker.h>
#include <TkUtil/InternalError.h>
#include <TkUtil/MemoryError.h>
#include <TkUtil/NumericConversions.h>
#include <TkUtil/NumericServices.h>

#include <QRegExpValidator>

#include <assert.h>
#include <string.h>

using namespace std;
using namespace TkUtil;

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


// ===========================================================================
//                        LA CLASSE QtCoordinatesTextField
// ===========================================================================

/**
 * Classe de validateur de type <I>QRegExp</I> tolérant une évolution de la
 * chaîne par son milieu. Par défaut la classe <I>QRegExpValidator</I> retourne
 * <I>Intermediate</I> si les modifications sont effectuées en fin de chaîne
 * (donc chaîne incomplète), mais pas en cas d'édition en son milieu. Cette
 * classe évalue où est effectuée la modification et retourne
 * <I>Intermediate</I> si elle est valide du début à ce point.
 */
class QtRegExpValidator : public QRegExpValidator
{
	public :

	QtRegExpValidator (QObject* parent)
		: QRegExpValidator (parent)
	{ }
	QtRegExpValidator (const QRegExp& exp, QObject* parent)
		: QRegExpValidator (exp, parent)
	{ }
	virtual ~QtRegExpValidator ( )
	{ }

	virtual QValidator::State validate (QString& input, int& pos ) const
	{
		if (regExp ( ).exactMatch(input))
			return Acceptable;
		else
		{
			const int	matched	= regExp ( ).matchedLength ( );
			if ((matched >= pos) && (matched < input.size ( )))
				return Intermediate;
			else
			{
				pos = input.size();
				return Invalid;
			}
		}

	}	// QValidator::State


	private :

	QtRegExpValidator& operator = (const QtRegExpValidator& validator)
	{ return *this; }
};	// class QtRegExpValidator


QtCoordinatesTextField::QtCoordinatesTextField (
		QWidget* parent, double x, double y, double z, unsigned short precision)
	: QtValidatedTextField (parent, false), _precision (precision)
{
	setCoordinates (x, y, z);
	const string	espaces ("[ ]*");
	const string	doubleExp (
						"[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
// Semble également OK :
//		"[+\\-]?(?:0|[1-9]\\d*)(?:\\.\\d*)?(?:[eE][+\\-]?\\d+)?");
	UTF8String	coordsExp;
	coordsExp << "\\(" << espaces << doubleExp << espaces
	          << "[,]" << espaces << doubleExp << espaces
	          << "[,]" << espaces << doubleExp << espaces << "\\)";
	QRegExp	expression (coordsExp.iso ( ).c_str ( ));
	QtRegExpValidator*	validator	= new QtRegExpValidator (expression, this);
	setValidator (validator);
}	// QtCoordinatesTextField::QtCoordinatesTextField


QtCoordinatesTextField::QtCoordinatesTextField (const QtCoordinatesTextField&)
	: QtValidatedTextField (0), _precision (9)
{
	assert (0 && "QtCoordinatesTextField copy constructor is not allowed.");
}	// QtCoordinatesTextField::QtCoordinatesTextField


QtCoordinatesTextField& QtCoordinatesTextField::operator = (
												const QtCoordinatesTextField&)
{
	assert (0 && "QtCoordinatesTextField assignment operator is not allowed.");
	return *this;
}	// QtCoordinatesTextField::operator =


QtCoordinatesTextField::~QtCoordinatesTextField ()
{
}	// QtCoordinatesTextField::~QtCoordinatesTextField


void QtCoordinatesTextField::getCoordinates (
										double& x, double& y, double& z) const
{
	const string			coords	= text ( ).toStdString ( );
	const string::size_type	beg		= coords.find ("(");
	const string::size_type	end		= coords.find (")");
	if ((string::npos == beg) || (string::npos == end))
	{
		UTF8String	error (charset);
		error << "Erreur lors de l'extraction des composantes des coordonnées\n"
		      << coords;
		throw Exception (error);
	}	// if ((string::npos == beg) || (string::npos == end))

	const string	values	= coords.substr (beg + 1, end - beg - 1);
	const string::size_type	c1	= coords.find (",");
	const string::size_type	c2	= coords.find (",", c1 + 1);
	if ((string::npos == c1) || (string::npos == c2))
	{
		UTF8String	error (charset);
		error << "Erreur lors de l'extraction des composantes des coordonnées\n"
		      << coords;
		throw Exception (error);
	}	// if ((string::npos == beg) || (string::npos == end))
	const string	xVal	= values.substr (0, c1 - 1);
	const string	yVal	= values.substr (c1, c2 - c1 - 1);
	const string	zVal	= values.substr (c2);
	x	= NumericConversions::strToDouble (xVal);
	y	= NumericConversions::strToDouble (yVal);
	z	= NumericConversions::strToDouble (zVal);
}	// QtCoordinatesTextField::getCoordinates


void QtCoordinatesTextField::getCoordinates (double coords [3]) const
{
	getCoordinates (coords [0], coords [1], coords [2]);
}	// QtCoordinatesTextField::getCoordinates


void QtCoordinatesTextField::setCoordinates (double x, double y, double z)
{
	UTF8String	coords (charset);
	coords << "("
	       << NumericConversions::shortestRepresentation (x, _precision) << ", "
	       << NumericConversions::shortestRepresentation (y, _precision) << ", "
	       << NumericConversions::shortestRepresentation (z, _precision) << ")";
	setText (coords.iso ( ));
}	// QtCoordinatesTextField::setCoordinates


void QtCoordinatesTextField::setCoordinates (const double coords [3])
{
	setCoordinates (coords [0], coords [1], coords [2]);
}	// QtCoordinatesTextField::setCoordinates


bool QtCoordinatesTextField::validate ( )
{
	try
	{
		QString				tValue	= text ( );
		int					pos		= 0;
		const QValidator*	v		= validator ( );

		if (0 == v)
		{
			INTERNAL_ERROR (exc, "Absence de validateur.", "QtCoordinatesTextField::validate")
			throw exc;
		}	// if (0 == v)

		if (QValidator::Acceptable != v->validate (tValue, pos))
		{
			UTF8String	errorMsg (charset);
			errorMsg << "Valeur "
					<< tValue.toStdString ( )
			        << " incorrecte. La valeur doit être du type (x, y, z)"
			        << "\n"
			        << "où x, y et z sont des réels en notation scientifique ("
			        << "ex : 0.123, -1.4e-2, ...).";
			throw Exception (errorMsg);
		}	// if (QValidator::Acceptable != v->validate (tValue, pos))
		setSkin (true);
	}
	catch (const Exception& exc)
	{
		setSkin (false);
		if (true == dialogOnError)
			QtMessageBox::displayErrorMessage (
							this, "Saisie invalide", exc.getFullMessage ( ));
		return false;
	}
	catch (...)
	{
		setSkin (false);
		if (true == dialogOnError)
			QtMessageBox::displayErrorMessage (
							this, "Saisie invalide", "Erreur non documentée.");
		return false;
	}

	return true;
}	// QtCoordinatesTextField::validate


