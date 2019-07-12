#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QtPropertyBrowser" for configuration "MinSizeRel"
set_property(TARGET QtPropertyBrowser APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(QtPropertyBrowser PROPERTIES
  IMPORTED_IMPLIB_MINSIZEREL "${QtPropertyBrowser_install_dir}/lib/QtPropertyBrowser.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_MINSIZEREL "Qt5::Widgets"
  IMPORTED_LOCATION_MINSIZEREL "${QtPropertyBrowser_install_dir}/bin/QtPropertyBrowser.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS QtPropertyBrowser )
list(APPEND _IMPORT_CHECK_FILES_FOR_QtPropertyBrowser "${QtPropertyBrowser_install_dir}/lib/QtPropertyBrowser.lib" "${QtPropertyBrowser_install_dir}/bin/QtPropertyBrowser.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
