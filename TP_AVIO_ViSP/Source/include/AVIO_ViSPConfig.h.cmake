#ifndef @PROJECT_NAME@Config_h
#define @PROJECT_NAME@Config_h


// @PROJECT_NAME@ major version.
#define @PROJECT_NAME@_VERSION_MAJOR @@PROJECT_NAME@_VERSION_MAJOR@

// @PROJECT_NAME@ minor version.
#define @PROJECT_NAME@_VERSION_MINOR @@PROJECT_NAME@_VERSION_MINOR@

// @PROJECT_NAME@ patch version.
#define @PROJECT_NAME@_VERSION_PATCH @@PROJECT_NAME@_VERSION_PATCH@

// @PROJECT_NAME@ version.
#define @PROJECT_NAME@_VERSION_FULL @@PROJECT_NAME@_VERSION_FULL@

// Defined if MSVC is the compiler
@CMAKEDEFINE@ @PROJECT_NAME@_USE_MSVC

// @PROJECT_NAME@ library is either compiled static or shared
// Used to set declspec(import, export) in headers if required under Windows
@CMAKEDEFINE@ @PROJECT_NAME@_BUILD_SHARED_LIBS

// Defined if UsNeedleSimulator library available.
@CMAKEDEFINE@ @PROJECT_NAME@_HAVE_UsNeedleSimulator

// Defined if ViSP library available.
@CMAKEDEFINE@ @PROJECT_NAME@_HAVE_VISP

#endif //@PROJECT_NAME@Config_h
