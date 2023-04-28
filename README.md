# GUIToolkits

Toolkits of various C++ utilities used in several LIHPC graphical softwares.

This project is a dependency of the [mgx](https://github.com/LIHPC-Computational-Geometry/mgx) product.

Its Spack and Cmake CIs are based on reusable workflows provided in [lihpccg-ci](https://github.com/LIHPC-Computational-Geometry/lihpccg-ci) project. 

## PRESENTATION

These libraries are organized thematically into components: system, GUI, python console, 3D VTK displays, user settings, client/server relationship via Corba, mesh quality GUI.

The installation of each of these libraries is optional, and involves the installation of its dependencies.

The **TkUtil** library provides basic classes useful for programming (UTF8String, Exception, Logs, number <=> string conversions, threads, file system, ...).

The **PythonUtil** library provides the PythonSession class for executing Python code (instruction, file) and retrieving outputs.

The **QtUtil** library offers generic Qt 5 components such as vector input panels, log display, help display dialog boxes, ...

The **QtNetwork** library provides a file selector dialog over the network.

The **QtPython** library offers python consoles to run python 2 and python 3 code in "normal" mode or in debug mode, with breakpoints.

The **VtkContrib** library provides location classes in a VTK scene, a point entry widget on an unstructured grid, and a torus implementation.

The **Annotations** library provides components for annotating a VTK scene.

The **QtVtk** library provides classes for printing and localizing a VTK scene in a Qt GUI.

The **Preferences** library provides user preference management classes. The persistence is in XML format, provided by Xerces C, and the UI panels by Qt.

La bibliothèque **QwtCharts** propose des services de gestion de graphiques type Histogrammes reposant sur la bibliothèque Qwt avec automatisation possible de certaines actions (gestion de menu de modification des graphiques, ...).

The **QQualif** library allows to graphically evaluate (histogram) the quality of unstructured GMDS, Lima++, or VTK meshes.

La bibliothèque **CorbaUtil** met à disposition des composants de base pour une application fonctionnant en mode client/serveur. Elle repose sur la biblliothèque omniORB.


## INSTALLATION

The installation relies on cmake executed in script mode (CMakeLists.txt file transmitted via the -P argument) in order to process command line arguments.

The command line arguments - besides the traditional cmake arguments - are as follows :

+ -tkutil: compiling/installing the TkUtil component.
+ -pythonutil: compiling/installing the PythonUtil component.
+ -qtutil: compiling/installing the QtUtil component.
+ -qtpython: compiling/installing the QtPython component.
+ -preferences: compiling/installing the Preferences component.
+ -plugin: compiling/installing the Plugin component.
+ -qwtcharts: compiling/installing the QwtCharts component.
+ -qqualif: compiling/installing the QQualif component.
    + options: [-DBUILD_GQLima:BOOL=ON][-DBUILD_GQGMDS:BOOL=ON][-DBUILD_GQVtk:BOOL=ON] to install Lima/GMDS/VTK drives respectively
    + The use of these options assumes that the corresponding libraries are accessible to cmake (CMAKE_PREFIX_PATH, ...).
+ -qtnetwork: compiling/installing the QtNetwork component.
+ -vtkcontrib: compiling/installing the VtkContrib component.
+ -qtvtk: compiling/installing the QtVtk component.
+ -annotations: compiling/installing the Annotations component.
+ -corbautil: compiling/installing the CorbaUtil component.
+ -DUSE_PYTHON_2:BOOL=ON or -DUSE_PYTHON_3:BOOL=ON : choose the major version of python to use.
The choice of a component involves that of its dependencies.

The project comes with a spack recipe whose variants correspond to the libraries to be installed.

Example :

```
cd /tmp/GUIToolkits
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_INSTALL_PREFIX=/opt/QQualif -DCMAKE_PREFIX_PATH="/opt/Lima/lib/Lima/cmake;/opt/Qualif/lib/qualifinternal/cmake" -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_TOOLCHAIN_FILE=/opt/configurations/Ubuntu_GNU7.cmake -C /opt/configurations/Ubuntu_GNU7_VTK711_OGL_QT5157.cmake -S /tmp/GUIToolkits -DUSE_PYTHON_2:BOOL=ON -B /tmp/qqualif_build -qqualif  -DBUILD_GQLima:BOOL=ON -P CMakeLists.txt
```
in spack, with the recipe provided :
```
spack install guitoolkits2 +qqualif +lima
```