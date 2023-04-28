#include "PrefsCore/DoubleNamedValue.h"

#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <sstream>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");

const string DoubleNamedValue::typeName ("double");


DoubleNamedValue& DoubleNamedValue::operator = (const DoubleNamedValue& dValue)
{
	copy (dValue);

	return *this;
}	// DoubleNamedValue::operator =


Element* DoubleNamedValue::clone ( ) const
{
	return new DoubleNamedValue (*this);
}	// DoubleNamedValue::clone


string DoubleNamedValue::getStrValue ( ) const
{
	UTF8String	value (charset);
	value << _value;

	return value.iso ( );
}	// DoubleNamedValue::getStrValue


void DoubleNamedValue::setStrValue (const string& value)
{
	checkForModification (true);	// v 5.7.0
	
	double		val;
	istringstream	stream (value.c_str ( ));

	stream >> val;
	if ((true == stream.fail ( )) || (true == stream.bad ( )) ||
	    (false == stream.eof ( )))
	{
		UTF8String	msg (charset);
		msg << "Impossible de convertir la chaine de caractères " << value << " en double.";
		throw Exception (msg);
	}	// if ((true == stream.fail ( )) || ...

	setValue (val);
}	// DoubleNamedValue::setStrValue


void DoubleNamedValue::setValue (double value)
{
	checkForModification (true);	// v 5.7.0
	
	_value	= value;
}	// DoubleNamedValue::setValue


void DoubleNamedValue::copy (const DoubleNamedValue& dValue)
{
	NamedValue::copy (dValue);
	setValue (dValue.getValue ( ));
}	// DoubleNamedValue


END_NAMESPACE_PREFS
