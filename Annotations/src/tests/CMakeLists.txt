include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/workarounds.cmake)

find_package(Qt5 COMPONENTS Core)

add_executable (annote main.cpp)

target_include_directories (annote PRIVATE ../Annotations/public)
target_compile_options (annote PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (annote PUBLIC Annotations)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (annote PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
