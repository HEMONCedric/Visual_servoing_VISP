#############################################################################
#
# $Id: FindUsTk.cmake,v 1.3 2008-12-19 14:24:13 fspindle Exp $
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
# This file is part of the ViSP toolkit
#
# This file may be distributed under the terms of the Q Public License
# as defined by Trolltech AS of Norway and appearing in the file
# LICENSE included in the packaging of this file.
#
# Licensees holding valid ViSP Professional Edition licenses may
# use this file in accordance with the ViSP Commercial License
# Agreement provided with the Software.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Contact visp@irisa.fr if any conditions of this licensing are
# not clear to you.
#
# Description:
# Try to find UsTk library.
#
# UsTk_FOUND
# UsTk_INCLUDE_DIR
# UsTk_LIBRARIES
#
#############################################################################

  
  FIND_PATH(UsTk_INCLUDE_DIR UsTk/usTkConfig.h
    $ENV{USTK_HOME}/include
    )
  #MESSAGE("DBG UsTk_INCLUDE_DIR=${UsTk_INCLUDE_DIR}")  
  
  FIND_LIBRARY(UsTk_LIBRARY
    NAMES UsTk
    PATHS 
    $ENV{USTK_HOME}/lib
    $ENV{USTK_HOME}/lib/Release
    )

  #MESSAGE("DBG UsTk_LIBRARY=${UsTk_LIBRARY}")
  
  ## --------------------------------
  
  IF(UsTk_LIBRARY)
    SET(UsTk_LIBRARIES ${UsTk_LIBRARY})
  ELSE(UsTk_LIBRARY)
    MESSAGE(WARNING "UsTk library not found.")
  ENDIF(UsTk_LIBRARY)
  
  IF(NOT UsTk_INCLUDE_DIR)
    MESSAGE(WARNING "UsTk include dir not found.")
  ENDIF(NOT UsTk_INCLUDE_DIR)
  
  IF(UsTk_LIBRARIES AND UsTk_INCLUDE_DIR)
    SET(UsTk_FOUND TRUE)
  ELSE(UsTk_LIBRARIES AND UsTk_INCLUDE_DIR)
    SET(UsTk_FOUND FALSE)
  ENDIF(UsTk_LIBRARIES AND UsTk_INCLUDE_DIR)
  
  MARK_AS_ADVANCED(
    UsTk_INCLUDE_DIR
    UsTk_LIBRARIES
    UsTk_LIBRARY
    )
