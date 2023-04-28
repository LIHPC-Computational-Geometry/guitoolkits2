#include "PrefsXerces/XercesSessionManager.h"
#include "PrefsXerces/XercesUtils.h"
#include <TkUtil/Exception.h>

#include <xercesc/util/PlatformUtils.hpp>

#include <iostream>

USING_STD
USING_UTIL
XERCES_CPP_NAMESPACE_USE

BEGIN_NAMESPACE_PREFS


size_t XercesSessionManager::_sessionCount	= 0;


XercesSessionManager::XercesSessionManager ( )
{
	if (0 == _sessionCount)
	{
		try
		{
			XMLPlatformUtils::Initialize ( );
		}
		catch (...)
		{
			throw Exception ("XercesSessionManager::XercesSessionManager : exception non documentee.");
		}
	}	// if (0 == _sessionCount)

	_sessionCount++;
}	// XercesSessionManager::XercesSessionManager


XercesSessionManager::~XercesSessionManager ( )
{
	_sessionCount--;

	if (0 == _sessionCount)
	{
		try
		{
			XMLPlatformUtils::Terminate ( );
		}
		catch (...)
		{
			cerr << "XercesSessionManager::~XercesSessionManager : exception non documentée." << endl;	// v 5.2.1
//			throw Exception ("XercesSessionManager::~XercesSessionManager : exception non documentee.");	// v 5.2.1
		}
	}	// if (0 == _sessionCount)
}	// XercesSessionManager::~XercesSessionManager


END_NAMESPACE_PREFS
