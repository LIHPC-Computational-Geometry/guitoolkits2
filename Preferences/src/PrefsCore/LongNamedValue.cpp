#include "PrefsCore/LongNamedValue.h"

#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <sstream>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");

const string LongNamedValue::typeName ("long");


LongNamedValue& LongNamedValue::operator = (const LongNamedValue& value)
{
	copy (value);
	return *this;
}	// LongNamedValue::operator =


Element* LongNamedValue::clone ( ) const
{
	return new LongNamedValue (*this);
}	// LongNamedValue::clone


string LongNamedValue::getStrValue ( ) const
{
	UTF8String	value (charset);
	value << _value;

	return value.iso ( );
}	// LongNamedValue::getStrValue


void LongNamedValue::setStrValue (const string& value)
{
	checkForModification (true);	// v 5.7.0
	
	long		val;
	istringstream	stream (value.c_str ( ));

	stream >> val;
	if ((true == stream.fail ( )) || (true == stream.bad ( )) || (false == stream.eof ( )))
	{
		UTF8String	msg (charset);
		msg << "Impossible de convertir la chaine de caractères " << value << " en entier long.";
		throw Exception (msg);
	}	// if ((true == stream.fail ( )) || ...

	setValue (val);
}	// LongNamedValue::setStrValue


void LongNamedValue::setValue (long value)
{
	checkForModification (true);	// v 5.7.0
	
	_value	= value;
}	// LongNamedValue::setValue


void LongNamedValue::copy (const LongNamedValue& value)
{
	NamedValue::copy (value);
	setValue (value.getValue ( ));
}	// LongNamedValue::copy


END_NAMESPACE_PREFS
