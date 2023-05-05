set (CURRENT_PACKAGE_NAME "QtPlugin")
set (CURRENT_PACKAGE_MACRO_NAME "QT_PLUGIN")

include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

find_package (PrefsQt 6 REQUIRED)

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (QtPlugin ${CPP_SOURCES} ${HEADERS})
set_property (TARGET QtPlugin PROPERTY AUTOMOC ON)
set (ALL_TARGETS QtPlugin)
set_property (TARGET QtPlugin PROPERTY VERSION ${QT_PLUGIN_VERSION})
set_property (TARGET QtPlugin PROPERTY SOVERSION ${QT_PLUGIN_MAJOR_VERSION})
set (QT_PLUGIN_PUBLIC_FLAGS -DQT_5)
set (QT_PLUGIN_PRIVATE_FLAGS -DPLUGIN_VERSION="${PLUGIN_VERSION}")

target_include_directories (QtPlugin PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_compile_definitions (QtPlugin PUBLIC ${QT_PLUGIN_PUBLIC_FLAGS})
target_compile_definitions (QtPlugin PRIVATE ${QT_PLUGIN_PRIVATE_FLAGS})
target_compile_options (QtPlugin PRIVATE ${SHARED_CFLAGS})	# Requested by Qt ...
target_include_directories (QtPlugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_link_libraries (QtPlugin PUBLIC PrefsQt::PrefsQt PluginCore)
# Etre capable une fois installée de retrouver TkUtil, Qt5*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libQtPlugin.so.5.0.0" to "") ...
set_target_properties (QtPlugin PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

# Inefficace ici : CMAKE_AUTOMOC => dans le CMakeLists.txt racine
#set (CMAKE_AUTOMOC ON)
#set (CMAKE_AUTORCC ON)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/QtPluginDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/QtPluginDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/QtPluginDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/QtPluginDistribution.h)

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
# Dont QtPluginDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtPluginDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${PLUGIN_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de QtPlugin :
set (INCLUDE_INSTALL_DIR include/)		# QtPlugin_INCLUDE_DIR avec QtPluginConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# QtPlugin_LIB_DIR avec QtPluginConfig.cmake.in
#set (RUNTIME_INSTALL_DIR bin/)		# QtPlugin_RUNTIME_DIR avec QtPluginConfig.cmake.in
# Création du fichier QtPluginConfig.cmake qui définira QtPlugin_INCLUDE_DIR, QtPlugin_LIB_DIR et QtPlugin_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

# Copie du fichier QtPluginTargets.cmake dans prefix/lib/QtPlugin/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers QtPluginConfig.cmake et QtPluginConfigVersion.cmake dans prefix/lib/QtPlugin/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
