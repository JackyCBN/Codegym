#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QtPropertyBrowser" for configuration "RelWithDebInfo"
set_property(TARGET QtPropertyBrowser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(QtPropertyBrowser PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "F:/Project/Github/Engine/Codegym/_builds/QtPropertyBrowser/lib/QtPropertyBrowser.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELWITHDEBINFO "Qt5::Widgets"
  IMPORTED_LOCATION_RELWITHDEBINFO "F:/Project/Github/Engine/Codegym/_builds/QtPropertyBrowser/bin/QtPropertyBrowser.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS QtPropertyBrowser )
list(APPEND _IMPORT_CHECK_FILES_FOR_QtPropertyBrowser "F:/Project/Github/Engine/Codegym/_builds/QtPropertyBrowser/lib/QtPropertyBrowser.lib" "F:/Project/Github/Engine/Codegym/_builds/QtPropertyBrowser/bin/QtPropertyBrowser.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
