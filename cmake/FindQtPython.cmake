if (NOT QtPython_FOUND)
    add_subdirectory(${CMAKE_SOURCE_DIR}/QtPython QtPython)
endif()
set(QtPython_FOUND YES)