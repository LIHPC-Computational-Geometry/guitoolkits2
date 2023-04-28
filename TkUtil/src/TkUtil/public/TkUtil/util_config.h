#ifndef UTIL_CONFIG_H
#define UTIL_CONFIG_H

#define USING_NAMESPACES	1


#ifdef USING_NAMESPACES

#define BEGIN_NAMESPACE_UTIL	namespace TkUtil {
#define END_NAMESPACE_UTIL	}

#ifndef IN_STD
#define IN_STD		std::
#define USING_STD	using namespace std;
#endif	// IN_STD

// Pour un probleme de compatibilite VTK/OSF. Par defaut les ios ne sont pas 
// dans std sous OSF, et VTK est compile sans option particuliere ...
#ifndef IN_STD_IO
#ifdef __osf__
#define IN_STD_IO
#else
#define IN_STD_IO	std::
#endif	// __osf__
#endif	// IN_STD_IO

#ifndef IN_UTIL
#define IN_UTIL		TkUtil::
#define USING_UTIL	using namespace TkUtil;
#endif	// IN_UTIL

#else

#error  "Absence d'espaces de nommages non encore geree."

#endif	// USING_NAMESPACES


#endif	// UTIL_CONFIG_H
