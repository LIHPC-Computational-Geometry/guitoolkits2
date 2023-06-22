if (NOT Plugin_FOUND)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Plugin Plugin)
endif()
set(Plugin_FOUND YES)