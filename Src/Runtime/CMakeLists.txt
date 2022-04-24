project("Runtime" VERSION 1.0)

#add_executable(Runtime ${PROJECT_SOURCE_DIR}/main.cpp)

#set(HPP_FILES ${PROJECT_SOURCE_DIR}/Base.h				${PROJECT_SOURCE_DIR}/ExportTest.h)
#set(SRC_FILES ${PROJECT_SOURCE_DIR}/Test.cpp			 ${PROJECT_SOURCE_DIR}/ExportTest.cpp)
			 
# files
FILE(GLOB_RECURSE SRC_FILES
	"${PROJECT_SOURCE_DIR}/*.cpp"
	"${PROJECT_SOURCE_DIR}/*.h")

# library
createSrcGroups(SRC_FILES)
			 
add_library(Runtime SHARED ${SRC_FILES})
add_library(Codegym::Runtime ALIAS Runtime)
set_target_properties (Runtime PROPERTIES DEBUG_POSTFIX ${CodeGym_DEBUG_POSTFIX})

target_compile_definitions(Runtime PRIVATE CG_DLL_EXPORTS)
target_compile_definitions(Runtime PUBLIC CG_DLL)

#target_include_directories(Runtime ${ConfigurationDirectory})
#target_include_directories(Runtime PUBLIC ${rttr_SOURCE_DIR}/..)
#add_library(reflection INTERFACE)
target_link_libraries(Runtime PUBLIC rttr_core_lib)
target_link_libraries(Runtime PUBLIC glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(Runtime PUBLIC ${OPENGL_LIBRARIES})

target_include_directories(Runtime PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
							   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
							   $<BUILD_INTERFACE:${ConfigurationDirectory}>
                               $<INSTALL_INTERFACE:include>)
							   