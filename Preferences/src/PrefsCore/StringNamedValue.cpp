#include "PrefsCore/StringNamedValue.h"


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS


const string StringNamedValue::typeName ("string");


StringNamedValue& StringNamedValue::operator = (const StringNamedValue& value)
{
	copy (value);
	return *this;
}	// StringNamedValue::operator =



Element* StringNamedValue::clone ( ) const
{
	return new StringNamedValue (*this);
}	// StringNamedValue::clone


string StringNamedValue::getStrValue ( ) const
{
	return getValue ( );
}	// StringNamedValue::getStrValue


void StringNamedValue::setStrValue (const string& value)
{
	setValue (value);
}	// StringNamedValue::setStrValue


void StringNamedValue::setValue (const UTF8String& value)
{
	checkForModification (true);	// v 5.7.0

	_value	= value;
}	// StringNamedValue::setValue


void StringNamedValue::copy (const StringNamedValue& value)
{
	NamedValue::copy (value);
	setValue (value.getValue ( ));
}	// StringNamedValue::copy


END_NAMESPACE_PREFS
