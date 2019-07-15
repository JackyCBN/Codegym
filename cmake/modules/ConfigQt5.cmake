# QT
set(QT_REQUIRED_VERSION "5.10.1")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(QT_PREFIX ${CMAKE_SOURCE_DIR}/Externals/Qt5/msvc2017_64/)
list(APPEND CMAKE_PREFIX_PATH ${QT_PREFIX})

find_package(Qt5 ${QT_REQUIRED_VERSION} REQUIRED COMPONENTS Core Gui Widgets)
include(QtPropertyBrowser)
#add_subdirectory(${CMAKE_SOURCE_DIR}/Externals/Qt5/QtPropertyBrowser ${CMAKE_BINARY_DIR}/test)
