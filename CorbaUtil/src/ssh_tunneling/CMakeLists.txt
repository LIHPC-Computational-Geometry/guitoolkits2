find_package (GUIToolkitsVariables)

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)

add_executable (oceane_server oceane_server.cpp)
add_executable (runner_client runner_client.cpp)

target_include_directories (oceane_server PRIVATE ../libtest ../CorbaUtil/public ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../CorbaUtil ${CMAKE_CURRENT_BINARY_DIR}/../libtest)
target_include_directories (runner_client PRIVATE ../libtest ../CorbaUtil/public ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../CorbaUtil ${CMAKE_CURRENT_BINARY_DIR}/../libtest)
target_link_libraries (oceane_server PUBLIC CorbaTest)
target_link_libraries (runner_client PUBLIC CorbaTest)

