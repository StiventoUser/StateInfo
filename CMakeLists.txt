cmake_minimum_required(VERSION 3.7)

project(StateInfo)

# Limit configurations to Debug and Release only (for multi-configuration CMake generators).
if (CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)
endif()

include_directories(include)

#
# Source files.
#

set(HEADERS
	include/StateException.h
	include/TypeToString.h
	)

set(SOURCES
	src/Main.cpp
	src/StateException.cpp
	)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

#
# Target.
#

add_executable(${PROJECT_NAME}
	${HEADERS}
	${SOURCES})
	