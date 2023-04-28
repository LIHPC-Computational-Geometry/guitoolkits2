#include "TkUtil/TraceLog.h"


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


TraceLog::TraceLog (
	const UTF8String& text, TYPE type, const UTF8String& file,
	unsigned long line)
	: DefaultLog (type, text), _fileName (file), _line (line),
	  _localizedText (charset)
{
	_localizedText << where ( ) << ' ' << text;
}	// TraceLog::TraceLog ( )


TraceLog::TraceLog (
	const string& text, TYPE type, const UTF8String& file,
	unsigned long line)
	: DefaultLog (type, text), _fileName (file), _line (line),
	  _localizedText (charset)
{
	_localizedText << where ( ) << ' ' << text;
}	// TraceLog::TraceLog ( )


TraceLog::TraceLog (const TraceLog& log)
	: DefaultLog (log), _fileName (log.getFile ( )), _line (log.getLine ( )),
	  _localizedText (log.getText ( ))
{
}	// TraceLog::TraceLog (const TraceLog&)


TraceLog& TraceLog::operator = (const TraceLog& log)
{
	if (&log != this)
	{
		DefaultLog::operator = (log);	
		_fileName		= log.getFile ( );
		_line			= log.getLine ( );
		_localizedText	= log.getText ( );
	}	// if (&log != this)

	return *this;
}	// TraceLog::operator =


TraceLog::~TraceLog ( )
{
}	// TraceLog::~TraceLog


Log* TraceLog::clone ( ) const
{
	return new TraceLog (*this);
}	// TraceLog::clone


UTF8String TraceLog::where ( ) const
{
	UTF8String	str (charset);
	str << getFile ( ) << " ligne " << getLine ( );

	return str;
}	// TraceLog::where



END_NAMESPACE_UTIL
