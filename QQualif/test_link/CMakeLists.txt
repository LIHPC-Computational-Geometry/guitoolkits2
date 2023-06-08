cmake_minimum_required(VERSION 3.20)
project(qqualif_test_link)

set(CMAKE_CXX_STANDARD 14)

find_package (QtQualif REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE QtQualif::QtQualif)

enable_testing()
add_test(
        NAME ${PROJECT_NAME}
        COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)