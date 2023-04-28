#include "TkUtil/ErrorLog.h"



BEGIN_NAMESPACE_UTIL


ErrorLog::ErrorLog (const UTF8String& text)
	: DefaultLog (ErrorLog::ERROR, text)
{
}	// ErrorLog::ErrorLog ( )


ErrorLog::ErrorLog (const IN_STD string& text)
	: DefaultLog (ErrorLog::ERROR, text)
{
}	// ErrorLog::ErrorLog ( )


ErrorLog::ErrorLog (const IN_STD exception& exc)
	: DefaultLog (
		ErrorLog::ERROR, UTF8String (exc.what ( ), UTF8String::defaultCharset))
{
}	// ErrorLog::ErrorLog ( )


ErrorLog::ErrorLog (const Exception& exc)
	: DefaultLog (ErrorLog::ERROR, exc.getFullMessage ( ))
{
}	// ErrorLog::ErrorLog ( )


ErrorLog::ErrorLog (const ErrorLog& log)
	: DefaultLog (log)
{
}	// ErrorLog::ErrorLog (const ErrorLog&)


ErrorLog& ErrorLog::operator = (const ErrorLog& log)
{
	if (&log != this)
	{
		DefaultLog::operator = (log);
	}	// if (&log != this)

	return *this;
}	// ErrorLog::operator =


ErrorLog::~ErrorLog ( )
{
}	// ErrorLog::~ErrorLog


Log* ErrorLog::clone ( ) const
{
	return new ErrorLog (*this);
}	// ErrorLog::clone



END_NAMESPACE_UTIL
