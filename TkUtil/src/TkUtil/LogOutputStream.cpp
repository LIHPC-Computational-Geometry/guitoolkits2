#include "TkUtil/LogOutputStream.h"

#include <assert.h>



BEGIN_NAMESPACE_UTIL


LogOutputStream::LogOutputStream ( )
#ifdef MULTITHREADED_APPLICATION
	: _mutex (new Mutex ( ))
#endif	// MULTITHREADED_APPLICATION
{
}	// LogOutputStream::LogOutputStream ( )


LogOutputStream::LogOutputStream (const LogOutputStream&)
#ifdef MULTITHREADED_APPLICATION
	: _mutex (new Mutex ( ))
#endif	// MULTITHREADED_APPLICATION
{
}	// LogOutputStream::LogOutputStream (const LogOutputStream&)


LogOutputStream& LogOutputStream::operator = (const LogOutputStream&)
{
	return *this;
}	// LogOutputStream::operator =


LogOutputStream::~LogOutputStream ( )
{
#ifdef MULTITHREADED_APPLICATION
	delete _mutex;
#endif	// MULTITHREADED_APPLICATION
}	// LogOutputStream::~LogOutputStream


LogOutputStream& operator << (LogOutputStream& stream, const Log& log)
{
	if (log.getType ( ) & stream.getMask ( ))
		stream.log (log);

	return stream;
}	// LogOutputStream& operator << (LogOutputStream& stream, const Log& log)


#ifdef MULTITHREADED_APPLICATION
Mutex* LogOutputStream::getMutex ( ) const
{
	assert (0 != _mutex);
	return _mutex;
}	// LogOutputStream::getMutex 
#endif	// MULTITHREADED_APPLICATION


END_NAMESPACE_UTIL
