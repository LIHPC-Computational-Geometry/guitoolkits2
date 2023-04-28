#include "PrefsCore/ColorNamedValue.h"

#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <sstream>
#include <functional>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");

const string ColorNamedValue::typeName ("color");


ColorNamedValue& ColorNamedValue::operator = (const ColorNamedValue& color)
{
	copy (color);

	return *this;
}	// ColorNamedValue::operator =


ColorNamedValue::ColorNamedValue (const UTF8String& name, double red, double green, double blue,
                                  const UTF8String& comment, bool overloadable, bool safeguardable)
	: DoubleTripletNamedValue (name, 0., 0., 0., comment, true, safeguardable)
{
	setValue (red, green, blue);
	setOverloadable (overloadable);
}	// ColorNamedValue::ColorNamedValue


Element* ColorNamedValue::clone ( ) const
{
	return new ColorNamedValue (*this);
}	// ColorNamedValue::clone


void ColorNamedValue::setStrValue (const string& value)
{
	checkForModification (true);	// v 5.7.0
	
	double		red, green, blue;
	char		p1, p2, c1, c2;
	istringstream	stream (value.c_str ( ));

	stream >> p1 >> red >> c1 >> green >> c2 >> blue >> p2;
//	if ((true == stream.fail ( )) || (true == stream.bad ( )) ||
//	    (false == stream.eof ( )))
// On n'a pas eof dans certains cas. Pourquoi ? Il n'y a plus rien a lire.
	if ((true == stream.fail ( )) || (true == stream.bad ( )))
	{
		UTF8String	msg (charset);
		msg << "Impossible de convertir la chaine de caractères "
		    << value << " en composantes d'une couleur.";
		throw Exception (msg);
	}	// if ((true == stream.fail ( )) || ...

	setValue (red, green, blue);
}	// ColorNamedValue::setStrValue


void ColorNamedValue::setValue (double red, double green, double blue)
{
	checkForModification (true);	// v 5.7.0
	
	evaluateValue (red, "rouge");
	evaluateValue (green, "vert");
	evaluateValue (blue, "bleu");

	DoubleTripletNamedValue::setValue (red, green, blue);
}	// ColorNamedValue::setValue


void ColorNamedValue::copy (const ColorNamedValue& color)
{
	DoubleTripletNamedValue::copy (color);
}	// ColorNamedValue::copy


void ColorNamedValue::evaluateValue (double value, const string& name)
{
	less_equal<double>	le;
	greater_equal<double>	ge;

	if ((true == le (value, 1.0)) && (true == ge (value, 0.)))
		return;

	UTF8String	errorMsg (charset);
	errorMsg << "La composante " << name << " (" << value << ") de la couleur " << getName ( ) << " n'est pas comprise entre 0 et 1.";
	throw Exception (errorMsg);
}	// ColorNamedValue::evaluateValue




END_NAMESPACE_PREFS
