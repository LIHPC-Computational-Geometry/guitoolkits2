set (CURRENT_PACKAGE_NAME "PrefsCore")
set (CURRENT_PACKAGE_MACRO_NAME "PREFERENCES")

include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

find_package (TkUtil 6 REQUIRED)
file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (PrefsCore ${CPP_SOURCES} ${HEADERS})
set (ALL_TARGETS PrefsCore)
set_property (TARGET PrefsCore PROPERTY VERSION ${PREFERENCES_VERSION})
set_property (TARGET PrefsCore PROPERTY SOVERSION ${PREFERENCES_MAJOR_VERSION})
set (PREFERENCES_PRIVATE_FLAGS -DPREFS_BASE_VERSION="${PREFERENCES_VERSION}")

target_include_directories (PrefsCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_compile_definitions (PrefsCore PRIVATE ${PREFERENCES_PRIVATE_FLAGS})
#target_compile_options (PrefsCore PRIVATE ${SHARED_CFLAGS})	# Requested by Qt ...
target_link_libraries (PrefsCore PUBLIC TkUtil::TkUtil)
# Etre capable une fois installée de retrouver TkUtil, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libPrefsCore.so.5.0.0" to "") ...
set_target_properties (PrefsCore PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/PrefsCoreDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/PrefsCoreDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/PrefsCoreDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/PrefsCoreDistribution.h)

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
# Dont PrefsCoreDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/PrefsCoreDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${PREFERENCES_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de PrefsCore :
set (INCLUDE_INSTALL_DIR include/)		# PrefsCore_INCLUDE_DIR avec PrefsCoreConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# PrefsCore_LIB_DIR avec PrefsCoreConfig.cmake.in
set (RUNTIME_INSTALL_DIR bin/)			# PrefsCore_RUNTIME_DIR avec PrefsCoreConfig.cmake.in
# Création du fichier PrefsCoreConfig.cmake qui définira PrefsCore_INCLUDE_DIR, PrefsCore_LIB_DIR et PrefsCore_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR RUNTIME_INSTALL_DIR)

# Copie du fichier PrefsCoreTargets.cmake dans prefix/lib/PrefsCore/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers PrefsCoreConfig.cmake et PrefsCoreConfigVersion.cmake dans prefix/lib/PrefsCore/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
