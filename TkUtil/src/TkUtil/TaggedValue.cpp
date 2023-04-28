#include "TkUtil/Exception.h"
#include "TkUtil/TaggedValue.h"

#include <sstream>



USING_UTIL
USING_STD

BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


TaggedValue::TaggedValue (const string& tag, const string& name)
	: _tag (tag), _name (name), _attributes ( )
{
	if (0 == _tag.length ( ))
		throw Exception (UTF8String ("Création d'une valeur balisée impossible : balise nulle.", charset));
	if (0 == _name.length ( ))
		throw Exception (UTF8String ("Création d'une valeur balisée impossible : nom nul.", charset));
}	// TaggedValue::TaggedValue (const string& tag, const string& name)


TaggedValue::TaggedValue (istream& stream)
	: _tag ( ), _name ( ), _attributes ( )
{
	stream >> _tag;
	if (true == stream.fail ( ))
		throw Exception (UTF8String ("Création d'une valeur balisée impossible : flux en erreur lors de la récupération de la balise.", charset));
	stream >> _name;
	if (true == stream.fail ( ))
		throw Exception (UTF8String ("Création d'une valeur balisée impossible : flux en erreur lors de la récupération du nom.", charset));

	while ((true == stream.good ( )) && (false == stream.eof ( )))
	{
		string	value;
		stream >> value;
		if ((false == stream.fail ( )) && (false == stream.bad ( )))
			_attributes.push_back (value);
	}	// while ((true == stream.good ( )) && (false == stream.eof ( )))
}	// TaggedValue::TaggedValue (istream& stream)


TaggedValue::TaggedValue (const TaggedValue& tv)
	: _tag (tv._tag), _name (tv._name), _attributes (tv._attributes)
{
}	// TaggedValue::TaggedValue (const TaggedValue& tv)


TaggedValue& TaggedValue::operator = (const TaggedValue& tv)
{
	if (&tv != this)
	{
		_tag			= tv._tag;
		_name			= tv._name;
		_attributes		= tv._attributes;
	}	// if (&tv != this)

	return *this;
}	// TaggedValue::operator =


TaggedValue::~TaggedValue ( )
{
}	// TaggedValue::~TaggedValue ( )


void TaggedValue::addAttribute (const IN_STD string& attr)
{
	_attributes.push_back (attr);
}	// TaggedValue::addAttribute


string TaggedValue::toString ( ) const
{
	ostringstream	stream;
	stream << getTag ( ) << ' ' << getName ( );

	const vector<string>&	attributes	= getAttributes ( );
	for (vector<string>::const_iterator it = attributes.begin ( );
	     attributes.end ( ) != it; it++)
		stream << ' ' << *it;

	return stream.str ( );
}	// TaggedValue::toString


bool TaggedValue::operator == (const TaggedValue& tv)
{
	if ((getTag ( ) != tv.getTag ( )) || (getName ( ) != tv.getName ( )))
		return false;

	const vector<string>&	left	= getAttributes ( );
	const vector<string>&	right	= tv.getAttributes ( );

	return left == right;
}	// TaggedValue::operator ==


END_NAMESPACE_UTIL
