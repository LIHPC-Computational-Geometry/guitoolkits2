enable_testing ( )

cmake_minimum_required (VERSION 3.14)

project (TkUtil LANGUAGES CXX)

include (GNUInstallDirs)

include (cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/common.cmake)

set (TkUtil_VERSION ${TK_UTIL_VERSION})


set (CMAKE_VERBOSE_MAKEFILE ON)
option (BUILD_SHARED_LIBS "Creation de bibliotheques dynamiques (defaut : ON)" ON)

add_subdirectory (src)

