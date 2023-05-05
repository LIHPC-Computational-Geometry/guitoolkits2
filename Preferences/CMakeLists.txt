cmake_minimum_required(VERSION 3.12)

project (Preferences CXX)

include (GNUInstallDirs)

include (cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)

set (Preferences_VERSION ${PREFERENCES_VERSION})

set (CMAKE_VERBOSE_MAKEFILE ON)
option (BUILD_SHARED_LIBS "Creation de bibliotheques dynamiques (defaut : ON)" ON)

add_subdirectory (src)
