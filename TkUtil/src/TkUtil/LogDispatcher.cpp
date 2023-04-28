#include "TkUtil/LogDispatcher.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <stdexcept>



BEGIN_NAMESPACE_UTIL


LogDispatcher::LogDispatcher (
	Log::TYPE mask, bool enableDate, bool enableTime, bool enableThreadID)
	: DefaultLogOutputStream (mask, enableDate, enableTime, enableThreadID),
	  _streams ( )
{
}	// LogDispatcher::LogDispatcher ( )


LogDispatcher::LogDispatcher (const LogDispatcher& stream)
	: DefaultLogOutputStream (stream)
{
	assert (0 && "LogDispatcher copy constructor is not allowed.");
}	// LogDispatcher::LogDispatcher (const LogDispatcher&)


LogDispatcher& LogDispatcher::operator = (const LogDispatcher&)
{
	assert (0 && "LogDispatcher assignment operator is not allowed.");

	return *this;
}	// LogDispatcher::operator =


LogDispatcher::~LogDispatcher ( )
{
	LOCK_LOG_STREAM

	for (IN_STD vector<LogOutputStream*>::iterator it = _streams.begin ( );
	     _streams.end ( ) != it; it++)
		delete *it;
}	// LogDispatcher::~LogDispatcher


void LogDispatcher::enableDate (bool enable)
{
	LOCK_LOG_STREAM

	DefaultLogOutputStream::enableDate (enable);
	for (IN_STD vector<LogOutputStream*>::const_iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		(*it)->enableDate (enable);
}	// LogDispatcher::enableDate


void LogDispatcher::enableTime (bool enable)
{
	LOCK_LOG_STREAM

	DefaultLogOutputStream::enableTime (enable);
	for (IN_STD vector<LogOutputStream*>::const_iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		(*it)->enableTime (enable);
}	// LogDispatcher::enableTime


void LogDispatcher::enableDate (bool date, bool time)
{
	LOCK_LOG_STREAM

	DefaultLogOutputStream::enableDate (date, time);
	for (IN_STD vector<LogOutputStream*>::const_iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		(*it)->enableDate (date, time);
}	// LogDispatcher::enableDate


void LogDispatcher::enableThreadID (bool enable)
{
	LOCK_LOG_STREAM

	DefaultLogOutputStream::enableThreadID (enable);
	for (IN_STD vector<LogOutputStream*>::const_iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		(*it)->enableThreadID (enable);
}	// LogDispatcher::enableThreadID


bool LogDispatcher::isEnabled ( ) const
{
	LOCK_LOG_STREAM

	for (IN_STD vector<LogOutputStream*>::const_iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		if (false == (*it)->isEnabled ( ))
			return false;

	return true;
}	// LogDispatcher::isEnabled


void LogDispatcher::log (const Log& log)
{
	LOCK_LOG_STREAM

	if (0 == (log.getType ( ) & getMask ( )))
		return;

	UTF8String	errors;	// v 4.0.0
	for (IN_STD vector<LogOutputStream*>::iterator it = _streams.begin ( );
	     _streams.end ( ) != it; it++)
	{
		try
		{
			(*it)->log (log);
		}
		catch (const Exception& exc)
		{
			if (false == errors.empty ( ))
				errors << "\n";
			errors << exc.getFullMessage ( );
		}
		catch (...)
		{
			if (false == errors.empty ( ))
				errors << "\n";
			errors << "Erreur non documentée.";
		}
	}	// for (IN_STD vector<LogOutputStream*>::iterator it = ...

	if ((false == errors.empty ( )) && (true == throwExceptionIfFails ( )))
	{	// v 4.0.0
		UTF8String	message;
		message << "Erreur(s) lors de l'écriture des logs :\n" << errors;
		throw Exception (message);
	}	// if ((false == errors.empty ( )) && ...
}	// LogDispatcher::log


void LogDispatcher::addStream (LogOutputStream* stream)
{
	LOCK_LOG_STREAM

	for (IN_STD vector<LogOutputStream*>::const_iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		if (*it == stream)
			return;

	_streams.push_back (stream);
}	// LogDispatcher::addStream


void LogDispatcher::releaseStream (const LogOutputStream* stream)
{
	LOCK_LOG_STREAM

	for (IN_STD vector<LogOutputStream*>::iterator it = _streams.begin( );
	     _streams.end ( ) != it; it++)
		if (*it == stream)
		{
			_streams.erase (it);
			return;
		}
}	// LogDispatcher::releaseStream


END_NAMESPACE_UTIL

