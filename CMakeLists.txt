enable_testing ( )

cmake_minimum_required (VERSION 3.20)

project (GUIToolkits2 LANGUAGES CXX)

function (syntax)
	message (NOTICE "Fichier CMakeLists.txt de compilation/installation de composantes du projet GUIToolkits2.")
	message (NOTICE "Les variables possibles sont: ")
	message (NOTICE "-DTK_UTIL:BOOL=ON             : compilation/installation de la composante TkUtil.")
	message (NOTICE "-DPYTHON_UTIL:BOOL=ON         : compilation/installation de la composante PythonUtil.")
	message (NOTICE "-DQT_UTIL:BOOL=ON             : compilation/installation de la composante QtUtil.")
	message (NOTICE "-DQT_PYTHON:BOOL=ON           : compilation/installation de la composante QtPython.")
	message (NOTICE "-DPREFERENCES:BOOL=ON         : compilation/installation de la composante Preferences.")
	message (NOTICE "-DPLUGIN:BOOL=ON              : compilation/installation de la composante Plugin.")
	message (NOTICE "-DQWT_CHARTS:BOOL=ON          : compilation/installation de la composante QwtCharts.")
	message (NOTICE "-DQQUALIF:BOOL=ON             : compilation/installation de la composante QQualif.")
	message (NOTICE "      options : [-DBUILD_GQLima:BOOL=ON][-DBUILD_GQGMDS:BOOL=ON][-DBUILD_GQVtk:BOOL=ON] pour installer respectivement les lecteurs Lima/GMDS/VTK")
	message (NOTICE "      L'usage de ces options suppose que les bibliothèques correspondantes soient accessibles à cmake (CMAKE_PREFIX_PATH, ...).")
	message (NOTICE "-DQT_NETWORK:BOOL=ON          : compilation/installation de la composante QtNetwork.")
	message (NOTICE "-DVTKCONTRIB:BOOL=ON          : compilation/installation de la composante VtkContrib.")
	message (NOTICE "-DQT_VTK:BOOL=ON              : compilation/installation de la composante QtVtk.")
	message (NOTICE "-DANNOTATIONS:BOOL=ON         : compilation/installation de la composante Annotations.")
	message (NOTICE "-DCORBA_UTIL:BOOL=ON          : compilation/installation de la composante CorbaUtil.")
	message (NOTICE "Le choix d'une composante entraîne celui de ses dépendances.")
	message (NOTICE "-DUSE_PYTHON_2:BOOL=ON ou -DUSE_PYTHON_3:BOOL=ON : choix de la version majeure de python à utiliser.")
endfunction (syntax)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set (GUIToolkitsVariables_CMAKE_DIR "${CMAKE_SOURCE_DIR}/GUIToolkitsVariables/cmake")

if (VTKCONTRIB)
	find_package(VtkContrib)
endif (VTKCONTRIB)

if (TK_UTIL)
	find_package(TkUtil)
endif (TK_UTIL)

if (PYTHON_UTIL)
	find_package(PythonUtil)
endif (PYTHON_UTIL)

if (QT_UTIL)
	find_package(QtUtil)
endif (QT_UTIL)

if (QT_VTK)
	find_package(QtVtk)
endif (QT_VTK)

if (ANNOTATIONS)
	find_package(Annotations)
endif (ANNOTATIONS)

if (QT_PYTHON)
	find_package(QtPython)
endif (QT_PYTHON)

if (PREFERENCES)
	find_package(PrefsQt)
endif (PREFERENCES)

if (PLUGIN)
	find_package(Plugin)
endif (PLUGIN)

if (QWT_CHARTS)
	find_package(QwtCharts)
endif (QWT_CHARTS)

if (QQUALIF)
	find_package(QQualif)
endif (QQUALIF)

if (QT_NETWORK)
	find_package(QtNetwork)
endif (QT_NETWORK)

if (CORBA_UTIL)
	find_package(CorbaUtil)
endif (CORBA_UTIL)
