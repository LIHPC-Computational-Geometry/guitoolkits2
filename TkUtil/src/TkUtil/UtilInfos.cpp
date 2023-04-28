#include "TkUtil/UtilInfos.h"

#include <assert.h>


BEGIN_NAMESPACE_UTIL


const Version UtilInfos::_version (UTIL_VERSION);


UtilInfos::UtilInfos ( )
{
	assert (0 && "UtilInfos::UtilInfos is not allowed.");
}


UtilInfos::UtilInfos (const UtilInfos&)
{
	assert (0 && "UtilInfos copy constructor is not allowed.");
}


UtilInfos& UtilInfos::operator = (const UtilInfos&)
{
	assert (0 && "UtilInfos operator = is not allowed.");
	return *this;
}


UtilInfos::~UtilInfos ( )
{
	assert (0 && "UtilInfos::~UtilInfos is not allowed.");
}



END_NAMESPACE_UTIL
