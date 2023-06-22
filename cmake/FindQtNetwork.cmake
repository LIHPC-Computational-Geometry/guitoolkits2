if (NOT QtNetwork_FOUND)
    add_subdirectory(${CMAKE_SOURCE_DIR}/QtNetwork QtNetwork)
endif()
set(QtNetwork_FOUND YES)