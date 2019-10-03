#######################################################
# cmake file for building ProtoDUNE_RecoMetrics
# @author Steve Green, Cambridge
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################

# project name
PROJECT( ProtoDUNE_RecoMetrics )

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

### DEPENDENCIES ############################################################

FIND_PACKAGE( ROOT REQUIRED )

FOREACH( pkg ROOT )
    IF( ${pkg}_FOUND )
        INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
        ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
    ENDIF()
ENDFOREACH()

INCLUDE_DIRECTORIES(BEFORE ./include ) # use BEFORE to fix problem on mac osx (case insensitive filesystem)

FILE(GLOB_RECURSE SRCS *.cc)

### LIBRARY AND TOOLS #######################################################

SET( CMAKE_CXX_FLAGS "-g -Wall -std=c++17" )

ADD_EXECUTABLE(Analysis ./src/Analysis.cxx ${SRCS})

ADD_EXECUTABLE(Differences ./src/Differences.cxx ${SRCS})

INSTALL( TARGETS Analysis Differences DESTINATION ${CMAKE_SOURCE_DIR}/bin PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE )

