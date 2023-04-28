#include "PrefsCore/PreferencesInfos.h"

#include <assert.h>


BEGIN_NAMESPACE_PREFS


const IN_UTIL Version PreferencesInfos::_version (PREFS_BASE_VERSION);


PreferencesInfos::PreferencesInfos ( )
{
	assert (0 && "PreferencesInfos::PreferencesInfos is not allowed.");
}


PreferencesInfos::PreferencesInfos (const PreferencesInfos&)
{
	assert (0 && "PreferencesInfos copy constructor is not allowed.");
}


PreferencesInfos& PreferencesInfos::operator = (const PreferencesInfos&)
{
	assert (0 && "PreferencesInfos operator = is not allowed.");
	return *this;
}


PreferencesInfos::~PreferencesInfos ( )
{
	assert (0 && "PreferencesInfos::~PreferencesInfos is not allowed.");
}



END_NAMESPACE_PREFS
