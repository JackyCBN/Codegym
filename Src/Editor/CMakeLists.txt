project("Editor" VERSION 1.0)

#set(HPP_FILES)
#set(SRC_FILES ${PROJECT_SOURCE_DIR}/WinMain.cpp)


# files
FILE(GLOB_RECURSE SRC_FILES
	"${PROJECT_SOURCE_DIR}/*.cpp"
	"${PROJECT_SOURCE_DIR}/*.h")

# library
createSrcGroups(SRC_FILES)

add_executable(Editor ${SRC_FILES})

set_target_properties (Editor PROPERTIES DEBUG_POSTFIX ${CodeGym_DEBUG_POSTFIX})
# Find Qt modules
include(ConfigQt5)

target_include_directories(Editor PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
							   $<BUILD_INTERFACE:${ConfigurationDirectory}>
                               $<INSTALL_INTERFACE:include>)

# Use the Widgets module from Qt 5.
target_link_libraries(Editor Qt5::Widgets)
target_link_libraries(Editor QtPropertyBrowser)
target_link_libraries(Editor Codegym::Runtime)

add_dependencies(Editor Runtime)

copy_shared_lib(Editor Qt5 Core)
copy_shared_lib(Editor Qt5 Gui)
copy_shared_lib(Editor Qt5 Widgets)
copy_shared_lib(Editor "" QtPropertyBrowser)

# Copy QT's plugins  We just copy plugins\platforms
# get Qt5::QWindowsIntegrationPlugin plugin info
get_qt5_plugin_info(Qt5::QWindowsIntegrationPlugin qtWindowsFileNameVar qtWindowsTypeVar qtWindowsDestVar)

# make sure create platforms dir in target destination
add_custom_command(TARGET Editor POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Editor>/${qtWindowsTypeVar}/)
	   
foreach(plugin ${Qt5Gui_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  get_qt5_plugin_info(${plugin} pluginFileNameVar pluginTypeVar pluginDestVar)
  #message("Core Plugin ${plugin} is at location ${_loc}")
  IF("${pluginTypeVar}" STREQUAL "${qtWindowsTypeVar}")
      withRelative_copy_shared_lib(Editor "" ${plugin} "${qtWindowsTypeVar}")
  ENDIF("${pluginTypeVar}" STREQUAL "${qtWindowsTypeVar}")
endforeach()

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(MSVC)