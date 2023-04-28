#include "PrefsXerces/XercesUtils.h"
#include <TkUtil/UTF8String.h>


#include <xercesc/dom/DOMLocator.hpp>
#include <string.h>




USING_STD
USING_UTIL
XERCES_CPP_NAMESPACE_USE

BEGIN_NAMESPACE_PREFS


bool ConfigErrorHandler::handleError (const DOMError& domError)
{
// Rem : non appele par Xerces 2.1 si les droits en ecriture sont interdits ...
	if (DOMError::DOM_SEVERITY_WARNING == domError.getSeverity ( ))
	{
		ConsoleOutput::cout ( ) << "WARNING : "
					<< StrX (domError.getMessage ( )).localForm ( );
		if ((0 != domError.getLocation ( )) && 
		    (0 != domError.getLocation ( )->getURI ( )))
		{
			ConsoleOutput::cout ( ) << " at " 
			     << StrX (domError.getLocation ( )->getURI ( )).localForm ( )
			     << co_endl;
		}
		return true;
	}
	else
		return false;
}	// ConfigErrorHandler::handleError


XMLTranscoder*	StrX::utf8Transcoder				= 0;		// v 5.6.0
char			StrX::bytesNodeValue [16*1024+4]	= { '\0' };	// v 5.6.0


StrX::StrX (const XMLCh* const toTranscode)		// v 5.6.0
{
	XMLSize_t				charsEaten	= 0;
	XMLTransService::Codes	resCode		= XMLTransService::InternalFailure;
	if (0 == utf8Transcoder)
		utf8Transcoder	= XMLPlatformUtils::fgTransService->makeNewTranscoderFor ("UTF-8", resCode, 16*1024);
		
	const XMLSize_t	count	= utf8Transcoder->transcodeTo (toTranscode, XMLString::stringLen (toTranscode), (XMLByte*)bytesNodeValue, 16*1024, charsEaten, XMLTranscoder::UnRep_Throw);
	fLocalForm	= new char [count + 1];
	memcpy (fLocalForm, bytesNodeValue, count);
	fLocalForm [count]	= '\0';
}	// StrX::StrX

END_NAMESPACE_PREFS
