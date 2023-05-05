set (CURRENT_PACKAGE_NAME "Annotations")
set (CURRENT_PACKAGE_MACRO_NAME "ANNOTATIONS")

include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

find_package (QtVtk 8 REQUIRED)
find_package (Qt5PrintSupport REQUIRED NO_CMAKE_SYSTEM_PATH)

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (Annotations ${CPP_SOURCES} ${HEADERS})
set_property (TARGET Annotations PROPERTY AUTOMOC ON)
set (ALL_TARGETS Annotations)
set_property (TARGET Annotations PROPERTY VERSION ${ANNOTATIONS_VERSION})
set_property (TARGET Annotations PROPERTY SOVERSION ${ANNOTATIONS_MAJOR_VERSION})
set (ANNOTATIONS_PUBLIC_FLAGS "${MANDATORY_CXX_OPTIONS}")
set (ANNOTATIONS_PRIVATE_FLAGS -DANNOTATIONS_VERSION="${ANNOTATIONS_VERSION}" "${MANDATORY_VTK_OPTIONS}")

target_include_directories (Annotations PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_include_directories (Annotations PUBLIC ${Qt5Widgets_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
target_compile_definitions (Annotations PUBLIC ${ANNOTATIONS_PUBLIC_FLAGS})
target_compile_definitions (Annotations PRIVATE ${ANNOTATIONS_PRIVATE_FLAGS})
target_compile_options (Annotations PRIVATE ${SHARED_CFLAGS})	# Requested by Qt ...
target_link_libraries (Annotations PUBLIC Qt5::PrintSupport QtVtk::QtVtk)	# En fait seules les libs VTK sont nécessaires ...

# Etre capable une fois installée de retrouver vtk*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libAnnotations.so.5.0.0" to "") ...
set_target_properties (Annotations PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)
# Inefficace ici : CMAKE_AUTOMOC => dans le CMakeLists.txt racine
#set (CMAKE_AUTOMOC ON)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/AnnotationsDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/AnnotationsDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/AnnotationsDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/AnnotationsDistribution.h)

# ConfigPackageLocation : c'est plus ou moins standardisé, le défaut étant lib/cmake.
# On nos recommande très vivement d'utiliser ce répertoire afin de limiter les soucis par la suite,
# notamment au niveau des vues.
set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})

# La génération des règles d'installation :
install (TARGETS ${ALL_TARGETS} EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
         
# Recopie des en-têtes dans le répertoire d'installation :
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})
# Dont AnnotationsDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/AnnotationsDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${ANNOTATIONS_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de Annotations :
set (INCLUDE_INSTALL_DIR include/)		# Annotations_INCLUDE_DIR avec AnnotationsConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# Annotations_LIB_DIR avec AnnotationsConfig.cmake.in
#set (RUNTIME_INSTALL_DIR bin/)		# Annotations_RUNTIME_DIR avec AnnotationsConfig.cmake.in
# Création du fichier AnnotationsConfig.cmake qui définira Annotations_INCLUDE_DIR, Annotations_LIB_DIR et Annotations_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

# Copie du fichier AnnotationsTargets.cmake dans prefix/lib/Annotations/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers AnnotationsConfig.cmake et AnnotationsConfigVersion.cmake dans prefix/lib/Annotations/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
