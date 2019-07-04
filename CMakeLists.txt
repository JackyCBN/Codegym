cmake_minimum_required (VERSION 3.8)

project("Codegym" VERSION 1.0)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/usr/local/share/OpenCV")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets REQUIRED)


# Find Qt5

find_package(Qt5Core 5.11.1 REQUIRED)

if(Qt5Core_FOUND)
	MESSAGE(STATUS "Found Qt ${Qt5Core_VERSION_STRING}")
else()
	MESSAGE(FATAL_ERROR "No Qt5 found")
endif()

add_executable(Codegym ${PROJECT_SOURCE_DIR}/src/test.cpp)