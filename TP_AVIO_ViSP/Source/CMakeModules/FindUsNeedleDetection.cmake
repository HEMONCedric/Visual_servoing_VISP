#############################################################################
#
# $Id: FindUsNeedleDetection.cmake,
#
# Copyright (C) 2008 Inria. All rights reserved.
#
# This software was developed at:
# IRISA/INRIA Rennes
# Projet Lagadic
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# http://www.irisa.fr/lagadic
#
# This file may be distributed under the terms of the Q Public License
# as defined by Trolltech AS of Norway and appearing in the file
# LICENSE included in the packaging of this file.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Contact jason.chevrie@irisa.fr if any conditions of this licensing are
# not clear to you.
#
# Description:
# Try to find UsNeedleDetection library.
#
# UsNeedleDetection_FOUND
# UsNeedleDetection_INCLUDE_DIRS
# UsNeedleDetection_LIBRARIES
#
#############################################################################

OPTION(USE_US_NEEDLE_DETECTION "Use UsNeedleDetection for needle tracking" TRUE)

IF(USE_US_NEEDLE_DETECTION)
  
FIND_PATH(UsNeedleDetection_INCLUDE_DIR UsNeedleDetection/usNeedleDetection.h
	$ENV{UsNeedleDetection_HOME}/include
	)
	#MESSAGE("DBG UsNeedleDetection_INCLUDE_DIR=${UsNeedleDetection_INCLUDE_DIR}")  

FIND_LIBRARY(UsNeedleDetection_LIBRARY
	NAMES UsNeedleDetection
	PATHS 
	$ENV{UsNeedleDetection_HOME}/lib
	$ENV{UsNeedleDetection_HOME}/lib/Release
	)

#MESSAGE("DBG UsNeedleDetection_LIBRARY=${UsNeedleDetection_LIBRARY}")

## --------------------------------

IF(UsNeedleDetection_INCLUDE_DIR)
	SET(UsNeedleDetection_INCLUDE_DIRS ${UsNeedleDetection_INCLUDE_DIR})
ELSE(UsNeedleDetection_INCLUDE_DIR)
    MESSAGE(WARNING "UsNeedleDetection include dir not found.")
ENDIF(UsNeedleDetection_INCLUDE_DIR)

IF(UsNeedleDetection_LIBRARY)
	SET(UsNeedleDetection_LIBRARIES ${UsNeedleDetection_LIBRARY})
ELSE(UsNeedleDetection_LIBRARY)
    MESSAGE(WARNING "UsNeedleDetection library not found.")
ENDIF(UsNeedleDetection_LIBRARY)

IF(UsNeedleDetection_INCLUDE_DIR AND UsNeedleDetection_LIBRARY)
    FIND_PACKAGE(UsTk)
	IF(UsTk_FOUND)
		SET(UsNeedleDetection_INCLUDE_DIRS ${UsNeedleDetection_INCLUDE_DIRS} ${UsTk_INCLUDE_DIR})
		SET(UsNeedleDetection_LIBRARIES ${UsNeedleDetection_LIBRARIES} ${UsTk_LIBRARIES})
		#INCLUDE_DIRECTORIES(${UsTk_INCLUDE_DIR})
		#LINK_DIRECTORIES(${UsTk_LIBRARIES})
	ENDIF(UsTk_FOUND)

	FIND_PACKAGE(VTK)
	IF(VTK_FOUND)
		SET(UsNeedleDetection_INCLUDE_DIRS ${UsNeedleDetection_INCLUDE_DIRS} ${VTK_INCLUDE_DIR})
		SET(UsNeedleDetection_LIBRARIES ${UsNeedleDetection_LIBRARIES} ${VTK_LIBRARIES})
  		INCLUDE(${VTK_USE_FILE})
	ENDIF(VTK_FOUND)

	IF(UsTk_FOUND AND VTK_FOUND)
		SET(UsNeedleDetection_FOUND TRUE)
	ELSE(UsTk_FOUND AND VTK_FOUND)
		SET(UsNeedleDetection_FOUND FALSE)	
	ENDIF(UsTk_FOUND AND VTK_FOUND)
ELSE(UsNeedleDetection_INCLUDE_DIR AND UsNeedleDetection_LIBRARY)
    SET(UsNeedleDetection_FOUND FALSE)
ENDIF(UsNeedleDetection_INCLUDE_DIR AND UsNeedleDetection_LIBRARY)
  
MARK_AS_ADVANCED(
    UsNeedleDetection_INCLUDE_DIR
    UsNeedleDetection_LIBRARY
    )

ENDIF(USE_US_NEEDLE_DETECTION)
