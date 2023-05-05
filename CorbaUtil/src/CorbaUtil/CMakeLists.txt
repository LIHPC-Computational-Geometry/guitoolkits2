set (CURRENT_PACKAGE_NAME "CorbaUtil")
set (CURRENT_PACKAGE_MACRO_NAME "CORBA_UTIL")

include (../../cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/omniORB4.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

#pkg_check_modules (omniORB4 REQUIRED omniORB4)
#pkg_check_modules (omnithread3 REQUIRED omnithread3)

file (GLOB IDLS ${CMAKE_INSTALL_IDLDIR}/${CURRENT_PACKAGE_NAME}/*.idl)
file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

set (list_idl "${IDLS}")
OMNIORB4_IDL_COMPILATION (list_idl)
LIST(APPEND CPP_SOURCES ${OMNIORB4_GENERATED_IDL_SOURCES})

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (CorbaUtil ${CPP_SOURCES} ${HEADERS})
set (ALL_TARGETS CorbaUtil)
set_property (TARGET CorbaUtil PROPERTY VERSION ${CORBA_UTIL_VERSION})
set_property (TARGET CorbaUtil PROPERTY SOVERSION ${CORBA_UTIL_MAJOR_VERSION})
# Etre capable une fois installée de retrouver TkUtil, libomniORB4.so, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libCorbaUtil.so.5.0.0" to "") ...
set_target_properties (CorbaUtil PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

# En fait on dépend de libomniDynamic4 qui dépend de ${omniORB4_LINK_LIBRARIES} fournit par pkg-config ...
#set (CORBA_LIB_DEPENDENCIES ${omniORB4_LINK_LIBRARIES} ${omniORB4_LIB_DIR}/libomniDynamic4.so)
set (CORBA_LIB_DEPENDENCIES ${omniORB4_LIB_DIR}/libomniDynamic4.so ${omniORB4_LINK_LIBRARIES})
set (CORBA_INC_DEPENDENCIES ${omniORB4_INCLUDE_DIRS} ${omniORB4_INCLUDE_DIRS}/omniORB4)
set (CORBA_UTIL_PUBLIC_FLAGS "-DOMNIORB -DMULTITHREADED_APPLICATION")
list (APPEND CORBA_UTIL_PUBLIC_FLAGS "-DOMNIORB_VERSION=\"${omniORB4_VERSION}\"" "-DOMNIORB_HEXADECIMAL_VERSION=${omniORB4_HEXADECIMAL_VERSION}")
set (CORBA_UTIL_PRIVATE_FLAGS "-DCORBA_UTIL_VERSION=\"${CORBA_UTIL_VERSION}\"")
target_compile_definitions (CorbaUtil PUBLIC ${CORBA_UTIL_PUBLIC_FLAGS})
target_compile_definitions (CorbaUtil PRIVATE ${CORBA_UTIL_PRIVATE_FLAGS})
target_include_directories (CorbaUtil PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
# Les include entre .hh générés automatiquement ne sont pas préfixés de CorbaUtil ... :
target_include_directories (CorbaUtil PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/${CURRENT_PACKAGE_NAME}>)
target_include_directories (CorbaUtil PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options (CorbaUtil PRIVATE ${SHARED_CFLAGS})	# Requested by omniORB
target_include_directories (CorbaUtil PUBLIC ${TkUtil_INCLUDE_DIR} ${CORBA_INC_DEPENDENCIES})
target_include_directories (CorbaUtil PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_link_libraries (CorbaUtil PUBLIC TkUtil ${CORBA_LIB_DEPENDENCIES})
target_link_libraries (CorbaUtil PUBLIC stdc++)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/CorbaUtilDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/CorbaUtilDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/CorbaUtilDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/CorbaUtilDistribution.h)

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
install (FILES ${IDLS} DESTINATION ${CMAKE_INSTALL_IDLDIR}/${CURRENT_PACKAGE_NAME})
install(FILES ${OMNIORB4_GENERATED_IDL_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})
# Dont CorbaUtilDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CorbaUtilDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${CORBA_UTIL_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de CorbaUtil :
set (INCLUDE_INSTALL_DIR include/)		# CorbaUtil_INCLUDE_DIR avec CorbaUtilConfig.cmake.in
set (IDL_INSTALL_DIR idl/)			# CorbaUtil_IDL_DIR avec CorbaUtilConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# CorbaUtil_LIB_DIR avec CorbaUtilConfig.cmake.in
# Création du fichier CorbaUtilConfig.cmake qui définira CorbaUtil_INCLUDE_DIR, CorbaUtil_LIB_DIR et CorbaUtil_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR IDL_INSTALL_DIR LIB_INSTALL_DIR)

# Copie du fichier CorbaUtilTargets.cmake dans prefix/lib/CorbaUtil/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers CorbaUtilConfig.cmake et CorbaUtilConfigVersion.cmake dans prefix/lib/CorbaUtil/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)

