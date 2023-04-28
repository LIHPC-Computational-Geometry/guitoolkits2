#
# Charge le pkgconfig xerces-c
#

# Chargement du package pkgconfig xerces-c. Requiert qu'il soit accessible via
# la variable d'environnement PKG_CONFIG_PATH :
include(FindPkgConfig)
pkg_check_modules (Xerces-C++ REQUIRED xerces-c)
#pkg_get_variable(Xerces-C++_INCLUDE_DIR Xerces-C++ includedir)
#pkg_get_variable(Xerces-C++_LIB_DIR Xerces-C++ libdir)
#pkg_get_variable(Xerces-C++_LIBRARIES Xerces-C++ xerces-c)

MESSAGE ("========================== Module XercesC++ ========================== ")
# Variables CMake prédéfinies :
MESSAGE ("Xerces-C++_INCLUDE_DIR   =" ${Xerces-C++_INCLUDE_DIR})
MESSAGE ("Xerces-C++_LIB_DIR       =" ${Xerces-C++_LIB_DIR})
MESSAGE ("Xerces-C++_LIBRARY_DIRS  =" ${Xerces-C++_LIB_DIR})
MESSAGE ("Xerces-C++_LIBRARIES     =" ${Xerces-C++_LIBRARIES})
MESSAGE ("Xerces-C++_LINK_LIBRARIES=" ${Xerces-C++_LINK_LIBRARIES})
MESSAGE ("Xerces-C++_CFLAGS        =" ${Xerces-C++_CFLAGS})
MESSAGE ("Xerces-C++_LDFLAGS       =" ${Xerces-C++_LDFLAGS})
MESSAGE ("========================== Module Xerces-C++ ========================== ")


