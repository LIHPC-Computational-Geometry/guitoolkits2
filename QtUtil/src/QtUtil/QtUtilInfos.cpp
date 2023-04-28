#include "QtUtil/QtUtilInfos.h"

#include <assert.h>

static const IN_UTIL Charset		charset ("àéèùô");


const IN_UTIL Version QtUtilInfos::_version (QT_UTIL_VERSION);


QtUtilInfos::QtUtilInfos ( )
{
	assert (0 && "QtUtilInfos::QtUtilInfos is not allowed.");
}


QtUtilInfos::QtUtilInfos (const QtUtilInfos&)
{
	assert (0 && "QtUtilInfos copy constructor is not allowed.");
}


QtUtilInfos& QtUtilInfos::operator = (const QtUtilInfos&)
{
	assert (0 && "QtUtilInfos operator = is not allowed.");
	return *this;
}


QtUtilInfos::~QtUtilInfos ( )
{
	assert (0 && "QtUtilInfos::~QtUtilInfos is not allowed.");
}


const IN_UTIL Version& QtUtilInfos::getVersion ( )
{
	return _version;
}	// QtUtilInfos::getVersion


IN_UTIL UTF8String QtUtilInfos::getDetailedVersion ( )
{
	IN_UTIL UTF8String	detailedVersion (getVersion ( ).getVersion ( ), charset);
	
#ifdef USE_QT_WEBENGINE
	detailedVersion << " (QtWebEngine)";
#else	// USE_QT_WEBENGINE
#	ifdef USE_QT_WEBKIT
		detailedVersion << " (QtWebkit)";
#		else
#			ifdef USE_QT_TEXT_BROWSER
				detailedVersion << " (QtTextBrowser)";
#			else
				detailedVersion << " (implémentation inconnue)";
#		endif	// USE_QT_TEXT_BROWSER
#	endif	// USE_QT_WEBKIT
#endif	// USE_QT_WEBENGINE

	return detailedVersion;
}	// QtUtilInfos::getDetailedVersion
