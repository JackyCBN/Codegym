# Enable testing using CTest
#enable_testing()

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release")

# Always include srcdir and builddir in include path
# This saves typing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} in about every subdir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# put the include dirs which are in the source or build tree
# before all other include dirs, so the headers in the sources
# are prefered over the already installed ones
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

# Use colored output
set(CMAKE_COLOR_MAKEFILE ON)

# Add an option to decide where to install the config files
if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.6.2)
    option(USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR "Prefer to install the <package>Config.cmake files to lib/cmake/<package> instead of lib/<package>/cmake" TRUE)
endif ()

if(MSVC)
    set(ESCAPE_CHAR ^)
endif()

set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )
set(LIB_INSTALL_DIR     "lib${LIB_SUFFIX}"  CACHE PATH "The subdirectory where libraries will be installed (default is ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})" FORCE)
set(INCLUDE_INSTALL_DIR "include"           CACHE PATH "The subdirectory where header files will be installed (default is ${CMAKE_INSTALL_PREFIX}/include)" FORCE)
