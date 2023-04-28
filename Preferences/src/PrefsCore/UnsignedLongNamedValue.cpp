#include "PrefsCore/UnsignedLongNamedValue.h"

#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <sstream>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");

const string UnsignedLongNamedValue::typeName ("unsigned long");


UnsignedLongNamedValue& UnsignedLongNamedValue::operator = (const UnsignedLongNamedValue& value)
{
	copy (value);
	return *this;
}	// UnsignedLongNamedValue::operator =


Element* UnsignedLongNamedValue::clone ( ) const
{
	return new UnsignedLongNamedValue (*this);
}	// UnsignedLongNamedValue::clone


string UnsignedLongNamedValue::getStrValue ( ) const
{
	UTF8String	value (charset);
	value << _value;

	return value.iso ( );
}	// UnsignedLongNamedValue::getStrValue


void UnsignedLongNamedValue::setStrValue (const string& value)
{
	checkForModification (true);	// v 5.7.0
	
	unsigned long	val;
	istringstream	stream (value.c_str ( ));

	stream >> val;
	if ((true == stream.fail ( )) || (true == stream.bad ( )) || (false == stream.eof ( )))
	{
		UTF8String	msg (charset);
		msg << "Impossible de convertir la chaine de caractères " << value << " en entier long non signé.";
		throw Exception (msg);
	}	// if ((true == stream.fail ( )) || ...

	setValue (val);
}	// UnsignedLongNamedValue::setStrValue


void UnsignedLongNamedValue::setValue (unsigned long value)
{
	checkForModification (true);	// v 5.7.0
	
	_value	= value;
}	// UnsignedLongNamedValue::setValue


void UnsignedLongNamedValue::copy (const UnsignedLongNamedValue& value)
{
	NamedValue::copy (value);
	setValue (value.getValue ( ));
}	// UnsignedLongNamedValue::copy



END_NAMESPACE_PREFS
