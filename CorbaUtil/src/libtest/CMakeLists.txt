find_package (GUIToolkitsVariables)

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/omniORB4.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)

file (GLOB IDLS ${CMAKE_INSTALL_IDLDIR}/*.idl)
file (GLOB CPP_SOURCES *.cpp)

set (list_idl "${IDLS}")
OMNIORB4_IDL_COMPILATION (list_idl)
LIST(APPEND CPP_SOURCES ${OMNIORB4_GENERATED_IDL_SOURCES})

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (CorbaTest ${CPP_SOURCES} ${HEADERS})
set (ALL_TARGETS CorbaTest)
set_property (TARGET CorbaTest PROPERTY VERSION ${CORBA_UTIL_VERSION})
set_property (TARGET CorbaTest PROPERTY SOVERSION ${CORBA_UTIL_MAJOR_VERSION})

target_include_directories (CorbaTest PRIVATE ../CorbaUtil/public ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (CorbaTest PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../CorbaUtil)
target_include_directories (CorbaTest PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries (CorbaTest PUBLIC CorbaUtil)


