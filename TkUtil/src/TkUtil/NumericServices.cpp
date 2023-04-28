/**
 * \file		NumericServices.cpp
 * \author		Charles PIGNEROL
 * \date		13/09/2013
 */

#include "TkUtil/NumericServices.h"

#include <TkUtil/NumericConversions.h>

#include <limits>
#include <cmath>


USING_STD


BEGIN_NAMESPACE_UTIL

const size_t		NumericServices::doubleSize			= sizeof (double);

const size_t		NumericServices::doubleDigitsMax	=
									numeric_limits<double>::digits10;
// Représentation : signe 1.mantisse EXXX
//#ifndef TOSTR_USES_CPP_IMPLEMENTATION			v 4.5.0
const size_t	NumericServices::doubleScientificNotationCharMax	= 36;
/*#else	// TOSTR_USES_CPP_IMPLEMENTATION
const size_t	NumericServices::doubleScientificNotationCharMax	= 
	1 + NumericServices::doubleDigitsMax + 
	NumericConversions::toStr (	// min -> signe en plus
			(unsigned long)numeric_limits<double>::min_exponent10).length ( );
#endif	// TOSTR_USES_CPP_IMPLEMENTATION */


const size_t		NumericServices::longDoubleSize			= sizeof (long double);
const size_t		NumericServices::longDoubleDigitsMax	=
									numeric_limits<long double>::digits10;
// Représentation : signe 1.mantisse EXXX
// #ifndef TOSTR_USES_CPP_IMPLEMENTATION			v 4.5.0
const size_t		NumericServices::longDoubleScientificNotationCharMax	= 39;
/* #else	// TOSTR_USES_CPP_IMPLEMENTATION
const size_t		NumericServices::longDoubleScientificNotationCharMax	=
	1 + NumericServices::longDoubleDigitsMax + 
	NumericConversions::toStr (	// min -> signe en plus
			(unsigned long)numeric_limits<long double>::min_exponent10).length ( );
#endif	// TOSTR_USES_CPP_IMPLEMENTATION	*/


const size_t		NumericServices::floatSize			= sizeof (float);
const size_t		NumericServices::floatDigitsMax	=
									numeric_limits<float>::digits10;
// Représentation : signe 1.mantisse EXXX
// #ifndef TOSTR_USES_CPP_IMPLEMENTATION			v 4.5.0
const size_t		NumericServices::floatScientificNotationCharMax	= 27;
/* #else	// TOSTR_USES_CPP_IMPLEMENTATION
const size_t		NumericServices::floatScientificNotationCharMax	=
	1 + NumericServices::floatDigitsMax + 
	NumericConversions::toStr (	// min -> signe en plus
			(unsigned long)numeric_limits<float>::min_exponent10).length ( );
#endif	// TOSTR_USES_CPP_IMPLEMENTATION */


const size_t		NumericServices::charSize			= sizeof (char);
const size_t		NumericServices::charDigitsMax		=
									numeric_limits<char>::digits10;

const size_t		NumericServices::unsignedCharSize			= sizeof(unsigned char);
const size_t		NumericServices::unsignedCharDigitsMax	=
									numeric_limits<unsigned char>::digits10;


const size_t		NumericServices::wcharSize			= sizeof (wchar_t);
const size_t		NumericServices::wcharDigitsMax		=
									numeric_limits<wchar_t>::digits10;


const size_t		NumericServices::shortSize			= sizeof (short);
const size_t		NumericServices::shortDigitsMax		=
									numeric_limits<short>::digits10;

const size_t		NumericServices::unsignedShortSize		= sizeof (unsigned short);
const size_t		NumericServices::unsignedShortDigitsMax	=
									numeric_limits<unsigned short>::digits10;


const size_t		NumericServices::intSize		= sizeof (int);
const size_t		NumericServices::intDigitsMax	=
									numeric_limits<int>::digits10;

const size_t		NumericServices::unsignedIntSize		= sizeof (unsigned int);
const size_t		NumericServices::unsignedIntDigitsMax	=
									numeric_limits<unsigned int>::digits10;


const size_t		NumericServices::longSize			= sizeof (long);
const size_t		NumericServices::longDigitsMax	=
									numeric_limits<long>::digits10;

const size_t		NumericServices::unsignedLongSize			= sizeof(unsigned long);
const size_t		NumericServices::unsignedLongDigitsMax	=
									numeric_limits<unsigned long>::digits10;


const size_t		NumericServices::longLongSize			= sizeof (long long);
const size_t		NumericServices::longLongDigitsMax	=
									numeric_limits<long long>::digits10;

const size_t		NumericServices::unsignedLongLongSize			=
											sizeof (unsigned long long);
const size_t		NumericServices::unsignedLongLongDigitsMax	=
									numeric_limits<unsigned long long>::digits10;


bool NumericServices::isValid (double value)
{
	if ((0 == isfinite (value)) ||
	    (0 != std::isnan (value)) || (0 != std::isinf (value)))
		return false;

	return true;
}	// NumericServices::isValid (double)


bool NumericServices::isValidAndNotMax (double value)
{
	if ((value == -doubleMachMax ( )) || (value == doubleMachMax ( )))
		return false;

	return isValid (value);
}	// NumericServices::isValidAndNotMax


bool NumericServices::isValid (float value)
{
	if ((0 == isfinite (value)) ||
	    (0 != std::isnan (value)) || (0 != std::isinf (value)))
		return false;

	return true;
}	// NumericServices::isValid (float)


bool NumericServices::isValidAndNotMax (float value)
{
	if ((value == -floatMachMax ( )) || (value == floatMachMax ( )))
		return false;

	return isValid (value);
}	// NumericServices::isValidAndNotMax


END_NAMESPACE_UTIL
