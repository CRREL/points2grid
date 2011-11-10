###############################################################################
#
# CMake module to search for PDAL library
#
# On success, the macro sets the following variables:
# PDAL_FOUND       = if the library found
# PDAL_LIBRARIES   = full path to the library
# PDAL_INCLUDE_DIR = where to find the library headers also defined,
#                       but not for general use are
# PDAL_LIBRARY     = where to find the PROJ.4 library.
# PDAL_VERSION     = version of library which was found, e.g. "1.2.5"
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
# Module source: http://github.com/mloskot/workshop/tree/master/cmake/
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
###############################################################################
MESSAGE(STATUS "Searching for PDAL ${PDAL_FIND_VERSION}+ library")

IF(PDAL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(PDAL_FIND_QUIETLY TRUE)
ENDIF()

IF(WIN32)
  SET(OSGEO4W_IMPORT_LIBRARY pdal)
  IF(DEFINED ENV{OSGEO4W_ROOT})
    SET(OSGEO4W_ROOT_DIR $ENV{OSGEO4W_ROOT})
    #MESSAGE(STATUS " FindPDAL: trying OSGeo4W using environment variable OSGEO4W_ROOT=$ENV{OSGEO4W_ROOT}")
  ELSE()
    SET(OSGEO4W_ROOT_DIR c:/OSGeo4W)
    #MESSAGE(STATUS " FindPDAL: trying OSGeo4W using default location OSGEO4W_ROOT=${OSGEO4W_ROOT_DIR}")
  ENDIF()
ENDIF()


FIND_PATH(PDAL_INCLUDE_DIR
  pdal.hpp
  PATH_PREFIXES pdal
  PATHS
  /usr/include
  /usr/local/include
  /tmp/lasjunk/include
  ${OSGEO4W_ROOT_DIR}/include)

if(WIN32)
    SET(PDAL_NAMES ${OSGEO4W_IMPORT_LIBRARY} pdal)
else()
    SET(PDAL_NAMES ${OSGEO4W_IMPORT_LIBRARY} pdal)
endif()

FIND_LIBRARY(PDAL_LIBRARY
  NAMES ${PDAL_NAMES}
  PATHS
  /usr/lib
  /usr/local/lib
  /tmp/lasjunk/lib
  ${OSGEO4W_ROOT_DIR}/lib)

IF(PDAL_FOUND)
  SET(PDAL_LIBRARIES ${PDAL_LIBRARY})
ENDIF()

IF(PDAL_INCLUDE_DIR)
  SET(PDAL_VERSION 0)

  SET(PDAL_VERSION_H "${PDAL_INCLUDE_DIR}/pdal/pdal_defines.h")
  FILE(READ ${PDAL_VERSION_H} PDAL_VERSION_H_CONTENTS)
  
  IF (DEFINED PDAL_VERSION_H_CONTENTS)
  
    # string will be something like "106000", which is xyyzzz (x=major, y=minor, z=patch)
    string(REGEX REPLACE ".*#define[ \t]PDAL_VERSION_MAJOR[ \t]+([0-9]+).*" "\\1" PDAL_VERSION_MAJOR "${PDAL_VERSION_H_CONTENTS}")
    string(REGEX REPLACE ".*#define[ \t]PDAL_VERSION_MINOR[ \t]+([0-9]+).*" "\\1" PDAL_VERSION_MINOR "${PDAL_VERSION_H_CONTENTS}")
    string(REGEX REPLACE ".*#define[ \t]PDAL_VERSION_PATCH[ \t]+([0-9]+).*" "\\1" PDAL_VERSION_PATCH "${PDAL_VERSION_H_CONTENTS}")
  #  message(FATAL_ERROR "${PDAL_VERSION_MAJOR}.${PDAL_VERSION_MINOR}.${PDAL_VERSION_PATCH}")

    if(NOT ${PDAL_VERSION_MAJOR} MATCHES "[0-9]+")
      message(FATAL_ERROR "PDAL version parsing failed for PDAL_VERSION_MAJOR!")
    endif()
    if(NOT ${PDAL_VERSION_MINOR} MATCHES "[0-9]+")
      message(FATAL_ERROR "PDAL version parsing failed for PDAL_VERSION_MINOR!")
    endif()
    if(NOT ${PDAL_VERSION_PATCH} MATCHES "[0-9]+")
      message(FATAL_ERROR "PDAL version parsing failed for PDAL_VERSION_PATCH!")
    endif()


    SET(PDAL_VERSION "${PDAL_VERSION_MAJOR}.${PDAL_VERSION_MINOR}.${PDAL_VERSION_PATCH}"
      CACHE INTERNAL "The version string for PDAL library")

    IF (PDAL_VERSION VERSION_EQUAL PDAL_FIND_VERSION OR
        PDAL_VERSION VERSION_GREATER PDAL_FIND_VERSION)
      MESSAGE(STATUS "Found PDAL version: ${PDAL_VERSION}")
    ELSE()
      MESSAGE(FATAL_ERROR "PDAL version check failed. Version ${PDAL_VERSION} was found, at least version ${PDAL_FIND_VERSION} is required")
    ENDIF()
  ELSE()
    MESSAGE(FATAL_ERROR "Failed to open ${PDAL_VERSION_H} file")
  ENDIF()

ENDIF()

# Handle the QUIETLY and REQUIRED arguments and set PDAL_FOUND to TRUE
# if all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PDAL DEFAULT_MSG PDAL_LIBRARY PDAL_INCLUDE_DIR)
