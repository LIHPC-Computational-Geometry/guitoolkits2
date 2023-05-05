include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

add_executable (tkencoding encoding.cpp Iso8859_1.cpp UTF_8.cpp)

target_include_directories (tkencoding PRIVATE ../TkUtil/public)
target_link_libraries (tkencoding PUBLIC TkUtil)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (tkencoding PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})

