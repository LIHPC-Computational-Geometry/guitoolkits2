#include "PrefsCore/BoolNamedValue.h"

#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");

const string BoolNamedValue::typeName ("bool");
const string BoolNamedValue::trueStr ("true");
const string BoolNamedValue::falseStr ("false");


BoolNamedValue& BoolNamedValue::operator = (const BoolNamedValue& namedValue)
{
	copy (namedValue);

	return *this;
}	// BoolNamedValue::operator =


Element* BoolNamedValue::clone ( ) const
{
	return new BoolNamedValue (*this);
}	// BoolNamedValue::clone


string BoolNamedValue::getStrValue ( ) const
{
	return true == getValue ( ) ? trueStr : falseStr;
}	// BoolNamedValue::getStrValue


void BoolNamedValue::setStrValue (const string& value)
{
	checkForModification (true);	// v 5.7.0
	
	if (trueStr == value)
		setValue (true);
	else if (falseStr == value)
		setValue (false);
	else
	{
		UTF8String	msg (charset);
		msg << "Impossible de convertir la chaine de caractères " << value << " en booléen.";
		throw Exception (msg);
	}
}	// BoolNamedValue::setStrValue


void BoolNamedValue::setValue (bool value)
{
	checkForModification (true);	// v 5.7.0
	
	_value	= value;
}	// BoolNamedValue::setValue


void BoolNamedValue::copy (const BoolNamedValue& elem)
{
	NamedValue::copy (elem);
	setValue (elem.getValue ( ));
}	// BoolNamedValue::copy




END_NAMESPACE_PREFS
