cmake_minimum_required (VERSION 3.14)

project (GUIToolkits CXX)
set (CURRENT_PACKAGE_NAME GUIToolkits)

include (GNUInstallDirs)

find_package (GUIToolkitsVariables)

include (cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/organization.cmake)

set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})


# Utilisation par d'autres composantes :
set (PACKAGE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${ConfigPackageLocation}/")		# GUIToolkits_CMAKE_DIR avec GUIToolkitsConfig.cmake.in

include(CMakePackageConfigHelpers)

# Création du fichier GUIToolkitsConfig.cmake qui définira GUIToolkits_CMAKE_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE} INSTALL_DESTINATION ${CMAKE_CMAKE_DIR} PATH_VARS PACKAGE_CMAKE_INSTALL_DIR GUI_TOOLKITS_VERSION)
	
# Copie du fichier GUIToolkitsVariablesConfig.cmake dans prefix/lib/GUIToolkitsVariables/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
