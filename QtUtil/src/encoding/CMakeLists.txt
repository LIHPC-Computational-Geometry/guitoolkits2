include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

find_package (Qt5Widgets 5 REQUIRED NO_CMAKE_SYSTEM_PATH)

add_executable (qtencoding QtEncodingMainWindow.cpp UTF_8.cpp Iso8859_1.cpp encoding.cpp)
set_property (TARGET qtencoding PROPERTY AUTOMOC ON)

target_include_directories (qtencoding PRIVATE ../QtUtil/public ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options (qtencoding PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (qtencoding PUBLIC QtUtil)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (qtencoding PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
