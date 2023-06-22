if (NOT QQualif_FOUND)
    add_subdirectory(${CMAKE_SOURCE_DIR}/QQualif QQualif)
endif()
set(QQualif_FOUND YES)