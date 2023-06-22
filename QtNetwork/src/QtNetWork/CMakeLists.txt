set (CURRENT_PACKAGE_NAME "QtNetWork")
set (CURRENT_PACKAGE_MACRO_NAME "QT_NETWORK")

find_package (GUIToolkitsVariables)

include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake/workarounds.cmake)

find_package(Qt5 COMPONENTS Widgets)
find_package(Qt5Core NO_CMAKE_SYSTEM_PATH)	# In order to enable moc ...
find_package (QtUtil 6 REQUIRED)

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp *.qrc ${HEADERS})

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (QtNetWork ${CPP_SOURCES} ${HEADERS})
set_property (TARGET QtNetWork PROPERTY AUTOMOC ON)
set_property (TARGET QtNetWork PROPERTY AUTORCC ON)
set (ALL_TARGETS QtNetWork)
set_property (TARGET QtNetWork PROPERTY VERSION ${QT_NETWORK_VERSION})
set_property (TARGET QtNetWork PROPERTY SOVERSION ${QT_NETWORK_MAJOR_VERSION})
set (QT_LIB_DEPENDENCIES Qt5::Widgets Qt5::Gui Qt5::Core)
set (QT_INC_DEPENDENCIES ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS})
# Rem : en Qt v 4.* on utilise -DUSE_QT_WEBKIT
set (QT_NETWORK_PUBLIC_FLAGS -DQT_5 -DUSE_QT_WEBENGINE)
set (QT_NETWORK_PUBLIC_FLAGS -DQT_5)
set (QT_NETWORK_PRIVATE_FLAGS -DQT_NETWORK_VERSION="${QT_NETWORK_VERSION}")

# REM : on avait historiquement l'instruction suivante, pour contourner un bogue icpc sur plateforme Bull :
# SET_SOURCE_FILES_PROPERTIES(${GSCC_CMPNTDIR}/QtNetworkFileDialog.cpp PROPERTIES COMPILE_FLAGS -O0)

target_include_directories (QtNetWork PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_compile_definitions (QtNetWork PUBLIC ${QT_NETWORK_PUBLIC_FLAGS})
target_compile_definitions (QtNetWork PRIVATE ${QT_NETWORK_PRIVATE_FLAGS})
target_compile_options (QtNetWork PRIVATE ${SHARED_CFLAGS})	# Requested by Qt ...
target_include_directories (QtNetWork PUBLIC ${QT_INC_DEPENDENCIES})
target_link_libraries (QtNetWork PUBLIC QtUtil::QtUtil ${QT_LIB_DEPENDENCIES})
# Etre capable une fois installée de retrouver QtUtil, TkUtil, Qt5*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libQtNetWork.so.5.0.0" to "") ...
set_target_properties (QtNetWork PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)
# Inefficace ici : CMAKE_AUTOMOC => dans le CMakeLists.txt racine
#set (CMAKE_AUTOMOC ON)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/QtNetWorkDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/QtNetWorkDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/QtNetWorkDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/QtNetWorkDistribution.h)

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
# Dont QtNetWorkDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtNetWorkDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${QT_NETWORK_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de QtNetWork :
set (INCLUDE_INSTALL_DIR include/)		# QtNetWork_INCLUDE_DIR avec QtNetWorkConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# QtNetWork_LIB_DIR avec QtNetWorkConfig.cmake.in
set (RUNTIME_INSTALL_DIR bin/)			# QtNetWork_RUNTIME_DIR avec QtNetWorkConfig.cmake.in
# Création du fichier QtNetWorkConfig.cmake qui définira QtNetWork_INCLUDE_DIR, QtNetWork_LIB_DIR et QtNetWork_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR RUNTIME_INSTALL_DIR)

# Copie du fichier QtNetWorkTargets.cmake dans prefix/lib/QtNetWork/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers QtNetWorkConfig.cmake et QtNetWorkConfigVersion.cmake dans prefix/lib/QtNetWork/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
