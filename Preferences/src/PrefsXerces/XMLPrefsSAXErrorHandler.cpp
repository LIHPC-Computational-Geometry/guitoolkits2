#include "PrefsXerces/XercesUtils.h"
#include "PrefsXerces/XMLPrefsSAXErrorHandler.h"

#include <sstream>

#include <assert.h>


USING_STD
USING_UTIL
XERCES_CPP_NAMESPACE_USE

BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


XMLPrefsSAXErrorHandler::XMLPrefsSAXErrorHandler ( )
	: ErrorHandler ( ), XMLPrefsErrorHandler ( )
{
}	// XMLPrefsSAXErrorHandler::XMLPrefsSAXErrorHandler ( )


XMLPrefsSAXErrorHandler::XMLPrefsSAXErrorHandler (const XMLPrefsSAXErrorHandler&)
	: ErrorHandler ( ), XMLPrefsErrorHandler ( )
{
	assert (0 && "XMLPrefsSAXErrorHandler copy constructor is not allowed.");
}	// XMLPrefsSAXErrorHandler::XMLPrefsSAXErrorHandler ( )


XMLPrefsSAXErrorHandler& XMLPrefsSAXErrorHandler::operator = (const XMLPrefsSAXErrorHandler&)
{
	assert (0 && "XMLPrefsSAXErrorHandler operator = is not allowed.");
	return *this;
}	// XMLPrefsSAXErrorHandler::operator =


XMLPrefsSAXErrorHandler::~XMLPrefsSAXErrorHandler ( )
{
}


void XMLPrefsSAXErrorHandler::warning (const SAXParseException& saxExc)
{
	addWarning (formatMessage (saxExc));
}	// XMLPrefsSAXErrorHandler::warning


void XMLPrefsSAXErrorHandler::error (const SAXParseException& saxExc)
{
	addError (formatMessage (saxExc));
}	// XMLPrefsSAXErrorHandler::error

void XMLPrefsSAXErrorHandler::fatalError (const SAXParseException& saxExc)
{
	addFatalError (formatMessage (saxExc));
}	// XMLPrefsSAXErrorHandler::fatalError


void XMLPrefsSAXErrorHandler::resetErrors ( )
{
	XMLPrefsErrorHandler::resetErrors ( );
}	// XMLPrefsSAXErrorHandler::resetErrors


UTF8String XMLPrefsSAXErrorHandler::formatMessage (const SAXParseException& saxExc) const
{
	ostringstream	stream;
	stream << "Ligne " << saxExc.getLineNumber ( )
	       << " Colonne " << saxExc.getColumnNumber ( )
	       << " : ";
	UTF8String	msg (stream.str ( ), charset);
//	msg	+= (const WChar_t*)saxExc.getMessage ( );
	msg	+= UTF8String ((const char*)saxExc.getMessage ( ), Charset::UTF_16);

	return msg;
}	// XMLPrefsSAXErrorHandler::formatMessage


END_NAMESPACE_PREFS

