#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QtPropertyBrowser" for configuration "Release"
set_property(TARGET QtPropertyBrowser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(QtPropertyBrowser PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${QtPropertyBrowser_install_dir}/lib/QtPropertyBrowser.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "Qt5::Widgets"
  IMPORTED_LOCATION_RELEASE "${QtPropertyBrowser_install_dir}/bin/QtPropertyBrowser.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS QtPropertyBrowser )
list(APPEND _IMPORT_CHECK_FILES_FOR_QtPropertyBrowser "${QtPropertyBrowser_install_dir}/lib/QtPropertyBrowser.lib" "${QtPropertyBrowser_install_dir}/bin/QtPropertyBrowser.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
