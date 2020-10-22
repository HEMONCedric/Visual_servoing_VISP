#ifndef AVIO_ViSPConfig_h
#define AVIO_ViSPConfig_h


// AVIO_ViSP major version.
#define AVIO_ViSP_VERSION_MAJOR 1

// AVIO_ViSP minor version.
#define AVIO_ViSP_VERSION_MINOR 0

// AVIO_ViSP patch version.
#define AVIO_ViSP_VERSION_PATCH 0

// AVIO_ViSP version.
#define AVIO_ViSP_VERSION_FULL 1.0.0

// Defined if MSVC is the compiler
/* #undef AVIO_ViSP_USE_MSVC */

// AVIO_ViSP library is either compiled static or shared
// Used to set declspec(import, export) in headers if required under Windows
/* #undef AVIO_ViSP_BUILD_SHARED_LIBS */

// Defined if UsNeedleSimulator library available.
/* #undef AVIO_ViSP_HAVE_UsNeedleSimulator */

// Defined if ViSP library available.
#define AVIO_ViSP_HAVE_VISP

#endif //AVIO_ViSPConfig_h
