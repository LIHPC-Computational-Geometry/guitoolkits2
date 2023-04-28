#include "PrefsXerces/XercesUtils.h"
#include "PrefsXerces/XMLPrefsDOMErrorHandler.h"

#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/util/XMLString.hpp>

#include <sstream>

#include <assert.h>


USING_STD
USING_UTIL
XERCES_CPP_NAMESPACE_USE

BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


XMLPrefsDOMErrorHandler::XMLPrefsDOMErrorHandler ( )
	: DOMErrorHandler ( ), XMLPrefsErrorHandler ( )
{
}	// XMLPrefsDOMErrorHandler::XMLPrefsDOMErrorHandler ( )


XMLPrefsDOMErrorHandler::XMLPrefsDOMErrorHandler (const XMLPrefsDOMErrorHandler&)
	: DOMErrorHandler ( ), XMLPrefsErrorHandler ( )
{
	assert (0 && "XMLPrefsDOMErrorHandler copy constructor is not allowed.");
}	// XMLPrefsDOMErrorHandler::XMLPrefsDOMErrorHandler ( )


XMLPrefsDOMErrorHandler& XMLPrefsDOMErrorHandler::operator = (const XMLPrefsDOMErrorHandler&)
{
	assert (0 && "XMLPrefsDOMErrorHandler operator = is not allowed.");
	return *this;
}	// XMLPrefsDOMErrorHandler::operator =


XMLPrefsDOMErrorHandler::~XMLPrefsDOMErrorHandler ( )
{
}


bool XMLPrefsDOMErrorHandler::handleError (const DOMError& error)
{
	switch (error.getSeverity ( ))
	{
		case DOMError::DOM_SEVERITY_WARNING		:
				addWarning (formatMessage (error));
				return true;
		case DOMError::DOM_SEVERITY_ERROR		:
				addError (formatMessage (error));
				return false;
		default									:
		case DOMError::DOM_SEVERITY_FATAL_ERROR	:
				addFatalError (formatMessage (error));
				return false;
	}	// switch (error.getSeverity ( ))
}	// XMLPrefsDOMErrorHandler::handleError


UTF8String XMLPrefsDOMErrorHandler::formatMessage (const DOMError& error) const
{
	UTF8String	message (charset);
	DOMLocator*		locator	= error.getLocation ( );
	if (0 != locator)
	{
		ostringstream	stream;
		stream << "Ligne " << locator->getLineNumber ( )
		       << " Colonne " << locator->getColumnNumber ( )
		       << " : ";
		message	+= stream.str ( );
	}	// if (0 != locator)
	else
		message	+= "Localisation inconnue : ";

//	message	+= (const WChar_t*)error.getMessage( );
	message	+= UTF8String((const char*)error.getMessage( ), Charset::UTF_16);

	return message;
}	// XMLPrefsDOMErrorHandler::formatMessage


END_NAMESPACE_PREFS

