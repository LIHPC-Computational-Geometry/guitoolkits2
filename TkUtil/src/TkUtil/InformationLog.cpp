#include "TkUtil/InformationLog.h"



BEGIN_NAMESPACE_UTIL


InformationLog::InformationLog (const UTF8String& text)
	: DefaultLog (InformationLog::INFORMATION, text)
{
}	// InformationLog::InformationLog ( )


InformationLog::InformationLog (const IN_STD string& text)
	: DefaultLog (InformationLog::INFORMATION, text)
{
}	// InformationLog::InformationLog ( )


InformationLog::InformationLog (const InformationLog& log)
	: DefaultLog (log)
{
}	// InformationLog::InformationLog (const InformationLog&)


InformationLog& InformationLog::operator = (const InformationLog& log)
{
	if (&log != this)
	{
		DefaultLog::operator = (log);
	}	// if (&log != this)

	return *this;
}	// InformationLog::operator =


InformationLog::~InformationLog ( )
{
}	// InformationLog::~InformationLog


Log* InformationLog::clone ( ) const
{
	return new InformationLog (*this);
}	// InformationLog::clone

END_NAMESPACE_UTIL
