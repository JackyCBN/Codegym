cmake_minimum_required (VERSION 3.8)

project("Codegym" VERSION 1.0)

# For debug libs and exes, add "d" postfix
set(CodeGym_DEBUG_POSTFIX "_d" CACHE STRING "Set debug library postfix")

set(EXTERNALS_DIR ${CMAKE_SOURCE_DIR}/Externals)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR} CACHE PATH "Directory where .exe and .dll files are dumped." FORCE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Default build type is RelWithDebInfo for release versions and Debug for developement versions
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
endif()

#utility_source
include(utility)

include(cmake/CPM.cmake)

add_subdirectory(Externals)

# macros
include(CodegymMacros)

# Cxx compiler config
include(CxxCompiler)

# This file contains all the needed initialization macros
include(CodegymDefaults)

add_subdirectory(Src)

# Always keep it last: CPack definitions file
include(CodegymDist)