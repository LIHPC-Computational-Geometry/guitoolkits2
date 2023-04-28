#include "TkUtil/Date.h"
#include "TkUtil/UTF8String.h"

#include <string.h>


USING_UTIL
USING_STD

BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


Date::Date ( )
	: _date ( )
{
	reset ( );
}	// Date::Date


Date::Date (const Date& date)
	: _date ( )
{
	memcpy (&_date, &date._date, sizeof (struct tm));
}	// Date::Date


Date& Date::operator = (const Date& date)
{
	if (&date != this)
	{
		memcpy (&_date, &date._date, sizeof (struct tm));
	}	// if (&date != this)

	return *this;
}	// Date::operator =


Date::~Date ( )
{
}	// Date::~Date ( )


string Date::getDate ( ) const
{
	UTF8String	str (charset);
	str << setw (2)
	    << (unsigned long)getDay ( ) << "/" << (unsigned long)getMonth ( )
	    << "/" << setw (4) << getYear ( );

//	return str.iso ( );
	return str.utf8 ( );	// v 5.6.1
}	// Date::getDate ( )


string Date::getTime ( ) const
{
	UTF8String	str (charset);
	str << setw (2) 
	    << (unsigned long)getHour ( ) << ":" << (unsigned long)getMinutes ( )
	    << ":" << (unsigned long)getSeconds ( );

//	return str.iso ( );
	return str.utf8 ( );	// v 5.6.1
}	// Date::getTime ( )


void Date::reset ( )
{
	time_t	current	= time (NULL);
	localtime_r (&current, &_date);
}	// Date::reset


END_NAMESPACE_UTIL
