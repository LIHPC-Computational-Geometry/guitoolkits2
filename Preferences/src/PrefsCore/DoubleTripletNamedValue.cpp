#include "PrefsCore/DoubleTripletNamedValue.h"

#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <sstream>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


const string DoubleTripletNamedValue::typeName ("doubleTriplet");


DoubleTripletNamedValue& DoubleTripletNamedValue::operator = (const DoubleTripletNamedValue& dtnv)
{
	copy (dtnv);
	return *this;
}	// DoubleTripletNamedValue::operator =


Element* DoubleTripletNamedValue::clone ( ) const
{
	return new DoubleTripletNamedValue (*this);
}	// DoubleTripletNamedValue::clone


string DoubleTripletNamedValue::getStrValue ( ) const
{
	UTF8String	value (charset);
	value << "(" << _x << ", " << _y << ", " << _z << ")";

	return value.iso ( );
}	// DoubleTripletNamedValue::getStrValue


void DoubleTripletNamedValue::setStrValue (const string& value)
{
	checkForModification (true);	// v 5.7.0
	
	double		x, y, z;
	char		p1, p2, c1, c2;
	istringstream	stream (value.c_str ( ));

	stream >> p1 >> x >> c1 >> y >> c2 >> z >> p2;
//	if ((true == stream.fail ( )) || (true == stream.bad ( )) || (false == stream.eof ( )))
// On n'a pas eof dans certains cas. Pourquoi ? Il n'y a plus rien a lire.
	if ((true == stream.fail ( )) || (true == stream.bad ( )))
	{
		UTF8String	msg (charset);
		msg << "Impossible de convertir la chaine de caractères " << value << " en un triplet de doubles.";
		throw Exception (msg);
	}	// if ((true == stream.fail ( )) || ...

	setValue (x, y, z);
}	// DoubleTripletNamedValue::setStrValue


void DoubleTripletNamedValue::setValue (double x, double y, double z)
{
	checkForModification (true);	// v 5.7.0
	
	_x	= x;
	_y	= y;
	_z	= z;
}	// DoubleTripletNamedValue::setValue


void DoubleTripletNamedValue::copy (const DoubleTripletNamedValue& dtnv)
{
	NamedValue::copy (dtnv);
	setValue (dtnv.getX ( ), dtnv.getY ( ), dtnv.getZ ( ));
}	// DoubleTripletNamedValue::copy


END_NAMESPACE_PREFS
