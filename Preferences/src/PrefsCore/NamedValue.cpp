#include "PrefsCore/NamedValue.h"


USING_STD


BEGIN_NAMESPACE_PREFS



NamedValue&  NamedValue::operator = (const  NamedValue& value)
{
	copy (value);
	return *this;
}	// NamedValue::operator =


void NamedValue::copy (const NamedValue& value)
{
	Element::copy (value);
}	// NamedValue::copy


void NamedValue::print (ostream& stream, size_t indent) const
{
	for (size_t i = 0; i < indent; i++)
		stream << '\t';
	stream << getName ( ).ascii ( ) << "\t\t\t= " << getStrValue ( ) << ";";
	if (0 != getComment ( ).length ( ))
		stream << "\t// " << getComment ( ).ascii ( );
}	// NamedValue::print


END_NAMESPACE_PREFS
