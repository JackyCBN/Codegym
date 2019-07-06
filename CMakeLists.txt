cmake_minimum_required (VERSION 3.8)

project("Codegym" VERSION 1.0)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Default build type is RelWithDebInfo for release versions and Debug for developement versions
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
endif()

# macros
include(CodegymMacros)

# Cxx compiler config
include(CxxCompiler)

# This file contains all the needed initialization macros
include(CodegymDefaults)

# Find Qt modules
include(ConfigQt5)

add_executable(Codegym WIN32 ${PROJECT_SOURCE_DIR}/src/test.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(Codegym Qt5::Widgets)
# Always keep it last: CPack definitions file
include(CodegymDist)