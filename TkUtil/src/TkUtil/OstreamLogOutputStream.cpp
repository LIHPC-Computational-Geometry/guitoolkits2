#include "TkUtil/OstreamLogOutputStream.h"
#include "TkUtil/AnsiEscapeCodes.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <stdexcept>
#include <iostream>


BEGIN_NAMESPACE_UTIL


OstreamLogOutputStream::OstreamLogOutputStream (
	IN_STD_IO ostream& stream, Log::TYPE mask, bool enableDate, bool enableTime,
	bool enableThreadID, bool enableColorization)
	: DefaultLogOutputStream (mask, enableDate, enableTime, enableThreadID),
	  _stream (stream), _enableColorization (enableColorization)
{
}	// OstreamLogOutputStream::OstreamLogOutputStream ( )


OstreamLogOutputStream::OstreamLogOutputStream (
	IN_STD_IO ostream& stream, const Charset& charset,
	Log::TYPE mask, bool enableDate, bool enableTime,
	bool enableThreadID, bool enableColorization)
	: DefaultLogOutputStream (
					charset, mask, enableDate, enableTime, enableThreadID),
	  _stream (stream), _enableColorization (enableColorization)
{
}	// OstreamLogOutputStream::OstreamLogOutputStream ( )


OstreamLogOutputStream::OstreamLogOutputStream (
										const OstreamLogOutputStream& stream)
	: DefaultLogOutputStream (stream),
	  _stream (IN_STD_IO cout), _enableColorization (stream._enableColorization)
{
	assert (0 && "OstreamLogOutputStream copy constructor is not allowed.");
}	// OstreamLogOutputStream::OstreamLogOutputStream (const OstreamLogOutputStream&)


OstreamLogOutputStream& OstreamLogOutputStream::operator = (
									const OstreamLogOutputStream&)
{
	assert (0 && "OstreamLogOutputStream assignment operator is not allowed.");

	return *this;
}	// OstreamLogOutputStream::operator =


OstreamLogOutputStream::~OstreamLogOutputStream ( )
{
}	// OstreamLogOutputStream::~OstreamLogOutputStream


bool OstreamLogOutputStream::isEnabled ( ) const
{
	return _stream.good ( );
}	// OstreamLogOutputStream::isEnabled


bool OstreamLogOutputStream::enableColorization ( ) const
{
	return _enableColorization;
}	// OstreamLogOutputStream::enableColorization


void OstreamLogOutputStream::enableColorization (bool enable)
{
	_enableColorization	= enable;
}	// OstreamLogOutputStream::enableColorization


void OstreamLogOutputStream::log (const Log& log)
{
	LOCK_LOG_STREAM

	if (0 == (log.getType ( ) & getMask ( )))
		return;

	try
	{
		TermAutoStyle	termAutoStyle;

		if (true == enableColorization ( ))
		{
			if (log.getType ( ) == Log::ERROR)
				termAutoStyle.setAttributes (_stream, AnsiEscapeCodes::redFg);
			else if (log.getType ( ) == Log::WARNING)
				termAutoStyle.setAttributes (_stream, AnsiEscapeCodes::blueFg);
			else if ((log.getType ( ) == Log::PROCESS) ||
			         (log.getType ( ) == Log::SCRIPTING))
				termAutoStyle.setAttributes (_stream, AnsiEscapeCodes::brownFg);
			else if ((log.getType ( ) == Log::TRACE_1) ||
			         (log.getType ( ) == Log::TRACE_2) ||
			         (log.getType ( ) == Log::TRACE_3) ||
			         (log.getType ( ) == Log::TRACE_4) ||
			         (log.getType ( ) == Log::TRACE_5))
				termAutoStyle.setAttributes (_stream, AnsiEscapeCodes::greenFg);
		}	// if (true == enableColorization ( ))

		_stream << format (log, getCharset ( )) << IN_STD_IO endl;
	}
	catch (const IN_UTIL Exception& exc)
	{
		if (true == throwExceptionIfFails ( ))
			throw;
		else
			IN_STD_IO cerr
			     << "Erreur lors de l'ecriture du message suivant dans un "
			     << "flux :" << IN_STD_IO endl
			     << log.getText ( ).utf8 ( ) << IN_STD_IO endl
			     << exc.getFullMessage ( ) << IN_STD_IO endl;
		
	}
	catch (...)
	{
		if (true == throwExceptionIfFails ( ))
			throw;
		else
			IN_STD_IO cerr
			     << "Erreur non documentée lors de l'ecriture du message "
			     << "suivant dans un flux :"
			     << IN_STD_IO endl << log.getText ( ).utf8 ( )
			     << IN_STD_IO endl;
	}
}	// OstreamLogOutputStream::log


END_NAMESPACE_UTIL

