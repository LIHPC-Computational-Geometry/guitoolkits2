#include "TkUtil/DefaultLogOutputStream.h"
#include "TkUtil/Exception.h"


USING_STD


BEGIN_NAMESPACE_UTIL


DefaultLogOutputStream::DefaultLogOutputStream (
		const Charset& charset, bool throwException,	// v 4.0.0
		Log::TYPE mask, bool enableDate, bool enableTime, bool enableThreadID)
	: LogOutputStream ( ),
	  _charset (charset), _throwException (throwException),	// v 4.0.0
	  _mask (mask), _enableDate (enableDate), _enableTime (enableTime),
	  _enableThreadID (enableThreadID)
{
}	// DefaultLogOutputStream::DefaultLogOutputStream


DefaultLogOutputStream::DefaultLogOutputStream (
		Log::TYPE mask, bool enableDate, bool enableTime, bool enableThreadID)
	: LogOutputStream ( ),
	  _charset ("àéèùô"), _throwException (false),	// v 4.0.0
	  _mask (mask), _enableDate (enableDate), _enableTime (enableTime),
	  _enableThreadID (enableThreadID)
{
}	// DefaultLogOutputStream::DefaultLogOutputStream


DefaultLogOutputStream::DefaultLogOutputStream (
				const DefaultLogOutputStream& stream)
	: LogOutputStream ( ),
	  _charset (stream._charset), _throwException (stream._throwException),
	  _mask (stream._mask), 
	  _enableDate (stream.displayDate ( )),
	  _enableTime (stream.displayTime ( )),
	  _enableThreadID (stream.displayThreadID ( ))
{
}	// DefaultLogOutputStream::DefaultLogOutputStream (const DefaultLogOutputStream&)


DefaultLogOutputStream& DefaultLogOutputStream::operator = (
				const DefaultLogOutputStream& stream)
{
	if (&stream != this)
	{
		_charset		= stream._charset;
		_throwException	= stream._throwException;
		_mask			= stream._mask;
		_enableDate		= stream.displayDate ( );
		_enableTime		= stream.displayTime ( );
		_enableThreadID	= stream.displayThreadID ( );
	}	// if (&stream != this)

	return *this;
}	// DefaultLogOutputStream::operator =


DefaultLogOutputStream::~DefaultLogOutputStream ( )
{
}	// DefaultLogOutputStream::~DefaultLogOutputStream


UTF8String DefaultLogOutputStream::format (const Log& log) const
{
	UTF8String		text;
	bool			first	= true;

	// On décalle les traces pour une meilleur lisibilité :
	size_t	shift	= 0;
	if (Log::TRACE_1 <= log.getType ( ))		shift	= 1;
	else if (Log::TRACE_2 <= log.getType ( ))	shift	= 2;
	else if (Log::TRACE_3 <= log.getType ( ))	shift	= 3;
	else if (Log::TRACE_4 <= log.getType ( ))	shift	= 4;
	else if (Log::TRACE_5 <= log.getType ( ))	shift	= 5;
	shift	*= 2;
	for (size_t i = 0; i < shift; i++)
		text << ' ';

	if (true == displayDate ( ))
	{
		text << log.getDate ( ).getDate ( );
		first	= false;
	}	// if (true == displayDate ( ))

	if (true == displayTime ( ))
	{
		if (false == first)
			text << ' ';
		text << log.getDate ( ).getTime ( );
		first	= false;
	}	// if (true == displayTime ( ))

	if (false == first)
		text << ' ';

#ifdef MULTITHREADED_APPLICATION
	if (true == displayThreadID ( ))
		text << "(THR " << (unsigned long)log.getThreadId ( ) << ") ";
#endif	// MULTITHREADED_APPLICATION

	text << log.getText ( );
	first	= false;

	return text;
}	// DefaultLogOutputStream::format


string DefaultLogOutputStream::format (
								const Log& log, const Charset& charset) const
{
	UTF8String	formatted	= format (log);
	string		required;

	switch (charset.charset ( ))
	{
		case Charset::ASCII		: required	= formatted.ascii ( );	break;
		case Charset::ISO_8859	: required	= formatted.iso ( );	break;
		case Charset::UTF_8		: required	= formatted.utf8 ( );	break;
		default			:
		{
			UTF8String	error (charset);
			error << "DefaultLogOutputStream::format : impossibilité de "
			      << "convertir le log dans le jeu de caractères "
			      << charset.name ( ) << " : jeu non supporté par ce service.";
			throw Exception (error);
		}
	}	// switch (charset.charset ( ))

	return required;
}	// DefaultLogOutputStream::format


END_NAMESPACE_UTIL
