cmake_minimum_required (VERSION 3.20)

project (GUIToolkits2 LANGUAGES CXX)

function (syntax)
	message (NOTICE "Fichier CMakeLists.txt de compilation/installation de composantes du projet GUIToolkits2.")
	message (NOTICE "Les variables possibles sont: ")
	message (NOTICE "-DTK_UTIL             : compilation/installation de la composante TkUtil.")
	message (NOTICE "-DPYTHON_UTIL         : compilation/installation de la composante PythonUtil.")
	message (NOTICE "-DQT_UTIL             : compilation/installation de la composante QtUtil.")
	message (NOTICE "-DQT_PYTHON           : compilation/installation de la composante QtPython.")
	message (NOTICE "-DPREFERENCES         : compilation/installation de la composante Preferences.")
	message (NOTICE "-DPLUGIN              : compilation/installation de la composante Plugin.")
	message (NOTICE "-DQWT_CHARTS          : compilation/installation de la composante QwtCharts.")
	message (NOTICE "-DQQUALIF             : compilation/installation de la composante QQualif.")
	message (NOTICE "      options : [-DBUILD_GQLima:BOOL=ON][-DBUILD_GQGMDS:BOOL=ON][-DBUILD_GQVtk:BOOL=ON] pour installer respectivement les lecteurs Lima/GMDS/VTK")
	message (NOTICE "      L'usage de ces options suppose que les bibliothèques correspondantes soient accessibles à cmake (CMAKE_PREFIX_PATH, ...).")
	message (NOTICE "-DQT_NETWORK          : compilation/installation de la composante QtNetwork.")
	message (NOTICE "-DVTKCONRIB           : compilation/installation de la composante VtkContrib.")
	message (NOTICE "-DQT_VTK              : compilation/installation de la composante QtVtk.")
	message (NOTICE "-DANNOTATIONS         : compilation/installation de la composante Annotations.")
	message (NOTICE "-DCORBA_UTIL          : compilation/installation de la composante CorbaUtil.")
	message (NOTICE "Le choix d'une composante entraîne celui de ses dépendances.")
	message (NOTICE "-DUSE_PYTHON_2:BOOL=ON ou -DUSE_PYTHON_3:BOOL=ON : choix de la version majeure de python à utiliser.")
endfunction (syntax)

# On créé la liste ordonnée des composantes à compiler/installer :
# VtkContrib doit passer avant QQualif car il effectue la détection de la version de VTK, nécessaire à QQualif si -DBUILD_GQVtk:BOOL=ON est positionné.
# C'est pas top clean (: ...
list (APPEND COMPONENTS )
if (VTKCONTRIB)
	list (APPEND COMPONENTS VtkContrib)
endif (VTKCONTRIB)

if (TK_UTIL)
	list (APPEND COMPONENTS TkUtil)
endif (TK_UTIL)

if (PYTHON_UTIL)
	list (APPEND COMPONENTS TkUtil PythonUtil)
endif (PYTHON_UTIL)

if (QT_UTIL)
	list (APPEND COMPONENTS TkUtil QtUtil)
endif (QT_UTIL)

if (QT_VTK)
	list (APPEND COMPONENTS VtkContrib TkUtil QtUtil QtVtk)
endif (QT_VTK)

if (ANNOTATIONS)
	list (APPEND COMPONENTS VtkContrib TkUtil QtUtil QtVtk Annotations)
endif (ANNOTATIONS)

if (QT_PYTHON)
	list (APPEND COMPONENTS TkUtil PythonUtil QtUtil QtPython)
endif (QT_PYTHON)

if (PREFERENCES)
	list (APPEND COMPONENTS TkUtil QtUtil Preferences)
endif (PREFERENCES)

if (PLUGIN)
	list (APPEND COMPONENTS TkUtil QtUtil Preferences Plugin)
endif (PLUGIN)

if (QWT_CHARTS)
	list (APPEND COMPONENTS TkUtil QtUtil QwtCharts)
endif (QWT_CHARTS)

if (QQUALIF)
	list (APPEND COMPONENTS TkUtil QtUtil QwtCharts QQualif)
endif (QQUALIF)

if (QT_NETWORK)
	list (APPEND COMPONENTS TkUtil QtUtil QtNetwork)
endif (QT_NETWORK)

if (CORBA_UTIL)
	list (APPEND COMPONENTS TkUtil CorbaUtil)
endif (CORBA_UTIL)

# A t'on sélectionné au moins une composante ?
list (FIND COMPONENTS TkUtil TK_UTIL)
list (FIND COMPONENTS VtkContrib VTKCONTRIB)
if ((TK_UTIL STREQUAL "-1") AND (VTKCONTRIB STREQUAL "-1"))
	syntax ( )
	message (FATAL_ERROR "Echec de la compilation/installation de composantes du projet GUIToolkits : absence de composante sélectionnée.")
endif ( )

list (REMOVE_DUPLICATES COMPONENTS)
message (STATUS "====> Composantes à installer : ${COMPONENTS}.")

foreach (component IN LISTS COMPONENTS)
	message (STATUS "\n====> ${component}")
	add_subdirectory(${component})
endforeach (component IN LISTS COMPONENTS)

