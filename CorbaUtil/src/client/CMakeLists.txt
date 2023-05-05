include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

add_executable (client oceane_clt.cpp)

target_include_directories (client PRIVATE ../libtest ../CorbaUtil/public ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../CorbaUtil ${CMAKE_CURRENT_BINARY_DIR}/../libtest)
target_link_libraries (client PUBLIC CorbaTest)

