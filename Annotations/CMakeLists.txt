cmake_minimum_required(VERSION 3.12)

# Rem : langage C ajouté pour VTK 9 car sinon il refuse de charger MPI_C demandé par ParallelMPI ...
project (Annotations C CXX)

include (GNUInstallDirs)

find_package (GUIToolkitsVariables)

include (cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)

set (AnnotationsVERSION ${ANNOTATIONS_VERSION})

set (CMAKE_AUTORCC ON)
    	
set (CMAKE_VERBOSE_MAKEFILE ON)
option (BUILD_SHARED_LIBS "Creation de bibliotheques dynamiques (defaut : ON)" ON)

add_subdirectory (src)
