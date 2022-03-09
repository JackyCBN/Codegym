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


target_include_directories(Editor PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
							   $<BUILD_INTERFACE:${ConfigurationDirectory}>
                               $<INSTALL_INTERFACE:include>)


target_link_libraries(Editor Codegym::Runtime)

add_dependencies(Editor Runtime)


# make sure create platforms dir in target destination
add_custom_command(TARGET Editor POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Editor>/${qtWindowsTypeVar}/)

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(MSVC)