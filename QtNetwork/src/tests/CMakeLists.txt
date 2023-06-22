find_package (GUIToolkitsVariables)

include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/workarounds.cmake)

find_package (Qt5Widgets 5 REQUIRED NO_CMAKE_SYSTEM_PATH)

add_executable (network_dlg QtNetworkApp.cpp network_dlg.cpp QtNetworkApp.h)
set_property (TARGET network_dlg PROPERTY AUTOMOC ON)

target_include_directories (network_dlg PRIVATE ../QtNetWork/public ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options (network_dlg PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (network_dlg PUBLIC QtNetWork)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (network_dlg PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
