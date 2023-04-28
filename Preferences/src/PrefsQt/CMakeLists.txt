set (CURRENT_PACKAGE_NAME "PrefsQt")
set (CURRENT_PACKAGE_MACRO_NAME "PREFERENCES")

find_package (GUIToolkitsVariables)

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)

find_package (QtUtil 6 REQUIRED)
find_package(Qt5Core 5 NO_CMAKE_SYSTEM_PATH)	# In order to enable moc ...

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (PrefsQt ${CPP_SOURCES} ${HEADERS})
set_property (TARGET PrefsQt PROPERTY AUTOMOC ON)
set (ALL_TARGETS PrefsQt)
set_property (TARGET PrefsQt PROPERTY VERSION ${QT_UTIL_VERSION})
set_property (TARGET PrefsQt PROPERTY SOVERSION ${QT_UTIL_MAJOR_VERSION})
set (PREFS_QT_PRIVATE_FLAGS -DQT_PREFS_VERSION="${PREFERENCES_VERSION}")

target_include_directories (PrefsQt PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_include_directories (PrefsQt PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../PrefsXerces/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_include_directories (PrefsQt PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../PrefsCore/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_compile_definitions (PrefsQt PRIVATE ${PREFS_QT_PRIVATE_FLAGS})
target_link_libraries (PrefsQt PUBLIC QtUtil::QtUtil PrefsXerces)
# Etre capable une fois installée de retrouver TkUtil, XercesC, Qt*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libPrefsQt.so.5.0.0" to "") ...
set_target_properties (PrefsQt PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
# Inefficace ici : CMAKE_AUTOMOC => dans le CMakeLists.txt racine
#set (CMAKE_AUTOMOC ON)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/PrefsQtDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/PrefsQtDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/PrefsQtDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/PrefsQtDistribution.h)

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
# Dont PrefsQtDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/PrefsQtDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${PREFERENCES_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de PrefsQt :
set (INCLUDE_INSTALL_DIR include/)		# PrefsQt_INCLUDE_DIR avec PrefsQtConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# PrefsQt_LIB_DIR avec PrefsQtConfig.cmake.in
set (RUNTIME_INSTALL_DIR bin/)			# PrefsQt_RUNTIME_DIR avec PrefsQtConfig.cmake.in
# Création du fichier PrefsQtConfig.cmake qui définira PrefsQt_INCLUDE_DIR, PrefsQt_LIB_DIR et PrefsQt_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR RUNTIME_INSTALL_DIR)

# Copie du fichier PrefsQtTargets.cmake dans prefix/lib/PrefsQt/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers PrefsQtConfig.cmake et PrefsQtConfigVersion.cmake dans prefix/lib/PrefsQt/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
