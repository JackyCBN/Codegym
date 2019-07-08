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

#include_directories(extern/rttr)
#set(RTTR_DIR ${CMAKE_SOURCE_DIR}/extern/rttr/_builds/install)
#list(APPEND CMAKE_PREFIX_PATH ${RTTR_DIR})
#set(RTTR_REQUIRED_VERSION "0.9.7 (64bit)")

find_package(RTTR 0.9.7 QUIET)
if(NOT RTTR_FOUND)
  message(STATUS "RTTR not found on system, will download source and build it")
  include(rttr.CMakeLists.txt)
endif()


#find_package(RTTR 0.9.7 CONFIG REQUIRED Core)
#add_executable(Codegym WIN32 ${PROJECT_SOURCE_DIR}/src/test.cpp)
add_executable(Codegym ${PROJECT_SOURCE_DIR}/src/test.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(Codegym Qt5::Widgets)
if(TARGET RTTR::Core)
     target_link_libraries(Codegym RTTR::Core)
else()
      target_link_libraries(Codegym RTTR::Core_Lib)
endif()

# Always keep it last: CPack definitions file
include(CodegymDist)