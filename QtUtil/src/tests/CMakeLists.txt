include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/workarounds.cmake)

find_package (Qt5Widgets 5 REQUIRED NO_CMAKE_SYSTEM_PATH)

add_executable (qt_tests QtAboutMainWindow.cpp about_dlg.cpp)
set_property (TARGET qt_tests PROPERTY AUTOMOC ON)

target_include_directories (qt_tests PRIVATE ../QtUtil/public ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options (qt_tests PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (qt_tests PUBLIC QtUtil)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (qt_tests PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
