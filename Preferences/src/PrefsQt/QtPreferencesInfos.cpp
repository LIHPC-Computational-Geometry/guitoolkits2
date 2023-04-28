#include "PrefsQt/QtPreferencesInfos.h"

#include <assert.h>


const IN_UTIL Version QtPreferencesInfos::_version (QT_PREFS_VERSION);


QtPreferencesInfos::QtPreferencesInfos ( )
{
	assert (0 && "QtPreferencesInfos::QtPreferencesInfos is not allowed.");
}


QtPreferencesInfos::QtPreferencesInfos (const QtPreferencesInfos&)
{
	assert (0 && "QtPreferencesInfos copy constructor is not allowed.");
}


QtPreferencesInfos& QtPreferencesInfos::operator = (const QtPreferencesInfos&)
{
	assert (0 && "QtPreferencesInfos operator = is not allowed.");
	return *this;
}


QtPreferencesInfos::~QtPreferencesInfos ( )
{
	assert (0 && "QtPreferencesInfos::~QtPreferencesInfos is not allowed.");
}

