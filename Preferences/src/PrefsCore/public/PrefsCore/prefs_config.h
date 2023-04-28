#ifndef PREFS_CONFIG_H
#define PREFS_CONFIG_H

#define USING_NAMESPACES	1


#ifdef USING_NAMESPACES

#define BEGIN_NAMESPACE_PREFS	namespace Preferences {
#define END_NAMESPACE_PREFS	}

#ifndef IN_STD
#define IN_STD		std::
#define USING_STD	using namespace std;
#endif	// IN_STD

#ifndef IN_PREFS
#define IN_PREFS		Preferences::
#define USING_PREFS	using namespace Preferences;
#endif	// IN_PREFS

#else

#error  "Absence d'espaces de nommages non encore geree."

#endif	// USING_NAMESPACES


#endif	// PREFS_CONFIG_H
