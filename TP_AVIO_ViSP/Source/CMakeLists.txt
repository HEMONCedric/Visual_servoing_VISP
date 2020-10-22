PROJECT(AVIO_ViSP)

# ccmake version that works
cmake_minimum_required(VERSION 2.6)

#-----------------------------------------------------------------------------
# ${PROJECT_NAME} version number.  An even minor number corresponds to releases.
SET(${PROJECT_NAME}_VERSION_MAJOR 1)
SET(${PROJECT_NAME}_VERSION_MINOR 0)
SET(${PROJECT_NAME}_VERSION_PATCH 0)
SET(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
SET(${PROJECT_NAME}_VERSION_FULL "${${PROJECT_NAME}_VERSION}.${${PROJECT_NAME}_VERSION_PATCH}")      

SET(${PROJECT_NAME}_INCLUDE_DIR "${${PROJECT_NAME}_BINARY_DIR}/include")

#-----------------------------------------------------------------------------

# The location in which to install ${PROJECT_NAME} libraries.
SET(LIBRARY_OUTPUT_PATH ${${PROJECT_NAME}_BINARY_DIR}/lib)

# The location in which to install ${PROJECT_NAME} binaries.
SET(BINARY_OUTPUT_PATH ${${PROJECT_NAME}_BINARY_DIR}/bin)

# the include directory we depend on
SET(${PROJECT_NAME}_INTERN_INCLUDE_DIR ${${PROJECT_NAME}_BINARY_DIR}/include)
# the third party include directories we depend on
SET(${PROJECT_NAME}_EXTERN_INCLUDE_DIR "")
# the third party libraries we depend on
SET(${PROJECT_NAME}_EXTERN_LIBS "")

# Get the OS
SET(OS ${CMAKE_SYSTEM_NAME})

# Add C++11
SET(CMAKE_CXX_FLAGS ${MAKE_CXX_FLAGS} -std=c++11)

## where are user-specific cmake modules
SET(CMAKE_MODULE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/CMakeModules)

INCLUDE(CheckLibraryExists)

# Create include directory which will contain all the headers
MAKE_DIRECTORY(${${PROJECT_NAME}_INCLUDE_DIR})

#--------------------------------------------------------------------
# Option management
#--------------------------------------------------------------------

# Choose static or shared libraries.
OPTION(BUILD_SHARED_LIBS "Build ${PROJECT_NAME} with shared libraries." OFF)

#----------------------------------------------------------------------
# Third party libraries
#----------------------------------------------------------------------

# UsNeedleSimulator
OPTION(USE_UsNeedleSimulator "Use UsNeedleSimulator library" FALSE)
IF(USE_UsNeedleSimulator)
	FIND_PACKAGE(UsNeedleSimulator REQUIRED)
	IF(UsNeedleSimulator_FOUND)
	  SET(${PROJECT_NAME}_EXTERN_INCLUDE_DIR ${${PROJECT_NAME}_EXTERN_INCLUDE_DIR} ${UsNeedleSimulator_INCLUDE_DIRS})
	  SET(${PROJECT_NAME}_EXTERN_LIBS ${${PROJECT_NAME}_EXTERN_LIBS} ${UsNeedleSimulator_LIBRARIES})
	  SET(${PROJECT_NAME}_HAVE_UsNeedleSimulator TRUE)
	  MESSAGE(STATUS "Found UsNeedleSimulator in ${UsNeedleSimulator_DIR}")
	ENDIF(UsNeedleSimulator_FOUND)
ENDIF(USE_UsNeedleSimulator)

# ViSP
OPTION(USE_ViSP "Use ViSP library" FALSE)
IF(UsNeedleSimulator_VISP_DIR)
	SET(USE_ViSP TRUE CACHE BOOL "Use ViSP library" FORCE)
	MESSAGE(STATUS "Found VISP via UsNeedleSimulator in ${UsNeedleSimulator_VISP_DIR}")
	SET(${PROJECT_NAME}_HAVE_VISP TRUE)
ELSEIF(USE_ViSP)
	FIND_PACKAGE(VISP REQUIRED)
	IF(VISP_FOUND)
      SET(${PROJECT_NAME}_EXTERN_INCLUDE_DIR ${${PROJECT_NAME}_EXTERN_INCLUDE_DIR} ${VISP_INCLUDE_DIRS})
	  SET(${PROJECT_NAME}_EXTERN_LIBS ${${PROJECT_NAME}_EXTERN_LIBS} ${VISP_LIBRARIES})
      MESSAGE(STATUS "Found VISP in ${VISP_DIR}")
	  SET(${PROJECT_NAME}_HAVE_VISP TRUE)
	ENDIF(VISP_FOUND)
ENDIF(UsNeedleSimulator_VISP_DIR)


#----------------------------------------------------------------------
# Generate the package dependent file include/${PROJECT_NAME}/${PROJECT_NAME}Config.h
#----------------------------------------------------------------------
SET(CMAKEDEFINE "#cmakedefine")
#Generate file include/${PROJECT_NAME}/${PROJECT_NAME}Config.h with #cmakedefine
CONFIGURE_FILE(${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}Config.h.cmake 
  ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h
)
#Generate final file include/${PROJECT_NAME}/${PROJECT_NAME}Config.h by prcessing #cmakedefine
CONFIGURE_FILE(${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h
  ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.h
)

#----------------------------------------------------------------------
# Propagation in sub dirs
#----------------------------------------------------------------------
ADD_SUBDIRECTORY(src)

