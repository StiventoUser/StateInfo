cmake_minimum_required(VERSION 3.7)

project(StateInfo)

# Limit configurations to Debug and Release only (for multi-configuration CMake generators).
if (CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ${BUILD_WITH_STATIC_CRT})

find_package(Boost REQUIRED COMPONENTS
	filesystem
	serialization
	random)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

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

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES})
