#include "PrefsXerces/XMLPrefsErrorHandler.h"

#include <assert.h>


USING_STD
USING_UTIL

BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


XMLPrefsErrorHandler::XMLPrefsErrorHandler ( )
	: _warnings ( ), _errors ( ), _fatalErrors ( )
{
}	// XMLPrefsErrorHandler::XMLPrefsErrorHandler ( )


XMLPrefsErrorHandler::XMLPrefsErrorHandler (const XMLPrefsErrorHandler&)
	: _warnings ( ), _errors ( ), _fatalErrors ( )
{
	assert (0 && "XMLPrefsErrorHandler copy constructor is not allowed.");
}	// XMLPrefsErrorHandler::XMLPrefsErrorHandler ( )


XMLPrefsErrorHandler& XMLPrefsErrorHandler::operator = (const XMLPrefsErrorHandler&)
{
	assert (0 && "XMLPrefsErrorHandler operator = is not allowed.");
	return *this;
}	// XMLPrefsErrorHandler::operator =


XMLPrefsErrorHandler::~XMLPrefsErrorHandler ( )
{
}


void XMLPrefsErrorHandler::addWarning (const UTF8String& warning)
{
	_warnings.push_back (warning);
}	// XMLPrefsErrorHandler::addWarning


void XMLPrefsErrorHandler::addError (const UTF8String& error)
{
	_errors.push_back (error);
}	// XMLPrefsErrorHandler::addError


void XMLPrefsErrorHandler::addFatalError (const UTF8String& error)
{
	_fatalErrors.push_back (error);
}	// XMLPrefsErrorHandler::addFatalError


void XMLPrefsErrorHandler::resetErrors ( )
{
	_warnings.clear ( );
	_errors.clear ( );
	_fatalErrors.clear ( );
}	// XMLPrefsErrorHandler::resetErrors


bool XMLPrefsErrorHandler::ok ( ) const
{
	if (false == hasWarnings ( ))
		return false;
	if (false == hasErrors ( ))
		return false;
	if (false == hasFatalErrors ( ))
		return false;

	return true;
}	// XMLPrefsErrorHandler::ok


UTF8String XMLPrefsErrorHandler::warningMessages ( ) const
{
	UTF8String	message (charset);

	for (vector<UTF8String>::const_iterator it = _warnings.begin ( );
	     _warnings.end ( ) != it; )
	{
		message += *it;
		it++;
		if (_warnings.end ( ) != it)
			message	+= '\n';
	}

	return message;
}	// XMLPrefsErrorHandler::warningMessages


UTF8String XMLPrefsErrorHandler::errorMessages ( ) const
{
	UTF8String	message (charset);

	for (vector<UTF8String>::const_iterator it = _errors.begin ( );
	     _errors.end ( ) != it; )
	{
		message += *it;
		it++;
		if (_errors.end ( ) != it)
			message	+= '\n';
	}

	return message;
}	// XMLPrefsErrorHandler::errorMessages


UTF8String XMLPrefsErrorHandler::fatalErrorMessages ( ) const
{
	UTF8String	message (charset);

	for (vector<UTF8String>::const_iterator it = _fatalErrors.begin ( );
	     _fatalErrors.end ( ) != it; )
	{
		message += *it;
		it++;
		if (_fatalErrors.end ( ) != it)
			message	+= '\n';
	}

	return message;
}	// XMLPrefsErrorHandler::fatalErrorMessages


UTF8String XMLPrefsErrorHandler::wholeMessages ( ) const
{
	UTF8String	message (charset);
	bool		newLine	= false;

	if (true == hasWarnings ( ))
	{
		newLine	= true;
		message	+= "Avertissements :\n" + warningMessages ( );
	}
	if (true == hasErrors ( ))
	{
		newLine	= true;
		message	+= "Erreurs :\n" + errorMessages ( );
	}
	if (true == hasFatalErrors ( ))
	{
		newLine	= true;
		message	+= "Erreurs fatales :\n" + fatalErrorMessages ( );
	}

	return message;
}	// XMLPrefsErrorHandler::wholeMessages


END_NAMESPACE_PREFS

