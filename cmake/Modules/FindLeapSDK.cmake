# Find LEAPMOTION
#
# This module defines
#  LEAPMOTION_FOUND
#  LEAPMOTION_INCLUDE_DIRS
#  LEAPMOTION_LIBRARIES
#
# Copyright (c) 2012 I-maginer
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place - Suite 330, Boston, MA 02111-1307, USA, or go to
# http://www.gnu.org/copyleft/lesser.txt
#

# On a new cmake run, we do not need to be verbose
IF(LEAPMOTION_INCLUDE_DIR AND LEAPMOTION_LIBRARY)
	SET(LEAPMOTION_FIND_QUIETLY TRUE)
ENDIF()

# If LEAPMOTION_ROOT was defined in the environment, use it.
if (NOT LEAPMOTION_ROOT)
  if(NOT "$ENV{LEAPMOTION_ROOT}" STREQUAL "")
    set(LEAPMOTION_ROOT $ENV{LEAPMOTION_ROOT})
  else()
    set(LEAPMOTION_ROOT $ENV{SCOL_DEPENDENCIES_PATH}/LeapDeveloperKit/LeapSDK)
  endif()
endif()

# concat all the search paths
IF(LEAPMOTION_ROOT)
	SET(LEAPMOTION_INCLUDE_SEARCH_DIRS
	  ${LEAPMOTION_INCLUDE_SEARCH_DIRS}
	  ${LEAPMOTION_ROOT}/include
  )
  SET(LEAPMOTION_LIBRARY_SEARCH_RELEASE_DIRS 
    ${LEAPMOTION_LIBRARY_SEARCH_DIRS}
    ${LEAPMOTION_ROOT}/lib/x86
  )
  SET(LEAPMOTION_LIBRARY_SEARCH_DEBUG_DIRS 
    ${LEAPMOTION_LIBRARY_SEARCH_DIRS}
    ${LEAPMOTION_ROOT}/lib/x86
  )
ENDIF()

# log message
IF (NOT LEAPMOTION_FIND_QUIETLY)
	MESSAGE(STATUS "Checking for LEAPMOTION library")
ENDIF()

# Search for header files
FIND_PATH(LEAPMOTION_INCLUDE_DIR Leap.h
  PATHS ${LEAPMOTION_INCLUDE_SEARCH_DIRS})

# Search for libraries files (release mode)
FIND_LIBRARY(LEAPMOTION_LIBRARY_RELEASE Leap
  PATHS ${LEAPMOTION_LIBRARY_SEARCH_RELEASE_DIRS})

# Search for libraries files (debug mode)
FIND_LIBRARY(LEAPMOTION_LIBRARY_DEBUG Leapd
  PATHS ${LEAPMOTION_LIBRARY_SEARCH_DEBUG_DIRS})

# Configure libraries for debug/release
SET(LEAPMOTION_INCLUDE_DIRS ${LEAPMOTION_INCLUDE_DIR} CACHE STRING "Directory containing LEAPMOTION header files")
SET(LEAPMOTION_LIBRARY debug ${LEAPMOTION_LIBRARY_DEBUG} optimized ${LEAPMOTION_LIBRARY_RELEASE})
SET(LEAPMOTION_LIBRARIES ${LEAPMOTION_LIBRARY} CACHE STRING "LEAPMOTION libraries files")

IF(LEAPMOTION_INCLUDE_DIR AND LEAPMOTION_LIBRARY)
	SET(LEAPMOTION_FOUND TRUE)
ENDIF()

# Hide those variables in GUI
SET(LEAPMOTION_INCLUDE_DIR ${LEAPMOTION_INCLUDE_DIR} CACHE INTERNAL "")
SET(LEAPMOTION_LIBRARY_RELEASE ${LEAPMOTION_LIBRARY_RELEASE} CACHE INTERNAL "")
SET(LEAPMOTION_LIBRARY_DEBUG ${LEAPMOTION_LIBRARY_DEBUG} CACHE INTERNAL "")
SET(LEAPMOTION_LIBRARY ${LEAPMOTION_LIBRARY} CACHE INTERNAL "")

# log find result
IF(LEAPMOTION_FOUND)
	IF(NOT LEAPMOTION_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries: ${LEAPMOTION_LIBRARIES}")
		MESSAGE(STATUS "  includes: ${LEAPMOTION_INCLUDE_DIRS}")
	ENDIF()
ELSE(LEAPMOTION_FOUND)
	IF(NOT LEAPMOTION_LIBRARIES)
		MESSAGE(STATUS, "LEAPMOTION library or one of it dependencies could not be found.")
	ENDIF()
	IF(NOT LEAPMOTION_INCLUDE_DIRS)
		MESSAGE(STATUS "LEAPMOTION include files could not be found.")
	ENDIF()
ENDIF(LEAPMOTION_FOUND)