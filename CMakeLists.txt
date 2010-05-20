CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BWTA_cmake)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF (${CMAKE_GENERATOR} MATCHES "NMake")
  SET(OUTDIR ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
ELSE ()
  SET(OUTDIR ${PROJECT_BINARY_DIR})
ENDIF ()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTDIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTDIR})

#-------------------------------------------------------------------------------------------------------------
OPTION(BWTA_FAST_BUILD "Decrease build time by building all source files at once" OFF)

#-------------------------------------------------------------------------------------------------------------
# Add actual sources
ADD_SUBDIRECTORY(src)

#-------------------------------------------------------------------------------------------------------------
# Includes
INCLUDE(setupeclipse)

#-------------------------------------------------------------------------------------------------------------
# Export

EXPORT(PACKAGE BWTA)
EXPORT(TARGETS BWTA_cmake FILE bwta-targets.cmake)

CONFIGURE_FILE(
  bwta-config.in.cmake
  bwta-config.cmake  
  IMMEDIATE @ONLY)
