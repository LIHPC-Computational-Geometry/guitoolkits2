#include "TkUtil/DefaultLog.h"


USING_STD


BEGIN_NAMESPACE_UTIL


DefaultLog::DefaultLog (DefaultLog::TYPE type, const UTF8String& text)
	: Log ( ), _type (type), _text (text), _date ( )
#ifdef MULTITHREADED_APPLICATION
	, _threadId (pthread_self ( ))
#endif	// MULTITHREADED_APPLICATION
{
}	// DefaultLog::DefaultLog ( )


DefaultLog::DefaultLog (DefaultLog::TYPE type, const string& text)
	: Log ( ), _type (type), _text (text), _date ( )
#ifdef MULTITHREADED_APPLICATION
	, _threadId (pthread_self ( ))
#endif	// MULTITHREADED_APPLICATION
{
}	// DefaultLog::DefaultLog ( )


DefaultLog::DefaultLog (const DefaultLog& log)
	: Log (log),  _type (log.getType ( )), _text (log.getText ( )),
	  _date (log.getDate ( ))
#ifdef MULTITHREADED_APPLICATION
	, _threadId (log.getThreadId ( ))
#endif	// MULTITHREADED_APPLICATION
{
}	// DefaultLog::DefaultLog (const DefaultLog&)


DefaultLog& DefaultLog::operator = (const DefaultLog& log)
{
	if (&log != this)
	{
		_type		= log.getType ( );
		_text		= log.getText ( );
		_date		= log.getDate ( );
#ifdef MULTITHREADED_APPLICATION
		_threadId	= log.getThreadId ( );
#endif	// MULTITHREADED_APPLICATION
	}	// if (&log != this)

	return *this;
}	// DefaultLog::operator =


DefaultLog::~DefaultLog ( )
{
}	// DefaultLog::~DefaultLog


void DefaultLog::setText (const UTF8String& text)
{
	_text	= text;
}	// DefaultLog::setText


END_NAMESPACE_UTIL
