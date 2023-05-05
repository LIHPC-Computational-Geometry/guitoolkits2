include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

add_executable (unix_server unix_server.cpp)
add_executable (unix_client unix_client.cpp)

target_include_directories (unix_server PRIVATE ../libtest ../CorbaUtil/public ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../CorbaUtil ${CMAKE_CURRENT_BINARY_DIR}/../libtest)
target_include_directories (unix_client PRIVATE ../libtest ../CorbaUtil/public ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../CorbaUtil ${CMAKE_CURRENT_BINARY_DIR}/../libtest)
target_link_libraries (unix_server PUBLIC CorbaTest)
target_link_libraries (unix_client PUBLIC CorbaTest)

