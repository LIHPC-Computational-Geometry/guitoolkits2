#include "TkUtil/WarningLog.h"



BEGIN_NAMESPACE_UTIL


WarningLog::WarningLog (const UTF8String& text)
	: DefaultLog (WarningLog::WARNING, text)
{
}	// WarningLog::WarningLog ( )


WarningLog::WarningLog (const IN_STD string& text)
	: DefaultLog (WarningLog::WARNING, text)
{
}	// WarningLog::WarningLog ( )


WarningLog::WarningLog (const std::exception& exc)
	: DefaultLog (
		WarningLog::WARNING, UTF8String(exc.what( ),UTF8String::defaultCharset))
{
}	// WarningLog::WarningLog ( )


WarningLog::WarningLog (const Exception& exc)
	: DefaultLog (WarningLog::WARNING, exc.getFullMessage ( ))
{
}	// WarningLog::WarningLog ( )


WarningLog::WarningLog (const WarningLog& log)
	: DefaultLog (log)
{
}	// WarningLog::WarningLog (const WarningLog&)


WarningLog& WarningLog::operator = (const WarningLog& log)
{
	if (&log != this)
	{
		DefaultLog::operator = (log);
	}	// if (&log != this)

	return *this;
}	// WarningLog::operator =


WarningLog::~WarningLog ( )
{
}	// WarningLog::~WarningLog


Log* WarningLog::clone ( ) const
{
	return new WarningLog (*this);
}	// WarningLog::clone



END_NAMESPACE_UTIL
