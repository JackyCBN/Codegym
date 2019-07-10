

include(ExternalProject)

set(configuration ${CMAKE_BUILD_TYPE})
ExternalProject_Add(rttr_project
    PREFIX rttr
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/rttr"
	UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${configuration} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_EXAMPLES=OFF -DBUILD_DOCUMENTATION=OFF -DBUILD_UNIT_TESTS=OFF -DBUILD_PACKAGE=OFF  --config ${configuration}
)

ExternalProject_Get_Property(rttr_project install_dir)
file(MAKE_DIRECTORY "${install_dir}/include")
file(MAKE_DIRECTORY "${install_dir}/lib")
# fails on Win: execute_process(COMMAND touch "${install_dir}/lib/librttr_core.a")
# CMake >=3.12: file(TOUCH "${install_dir}/lib/librttr_core.a")
#if(NOT EXISTS "${install_dir}/lib/librttr_core.a")
    # file(WRITE "${install_dir}/lib/librttr_core.a" "dummy file to be replaced by build")
#endif()

add_library(RTTR::Core STATIC IMPORTED)
add_dependencies(RTTR::Core rttr_project)
target_compile_definitions(RTTR::Core INTERFACE RTTR_DLL)

message(STATUS "${install_dir}/lib/rttr_core_d${CMAKE_STATIC_LIBRARY_SUFFIX}")
#set_property(TARGET RTTR::Core PROPERTY IMPORTED_LOCATION "${install_dir}/lib/rttr_core_d${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_target_properties(RTTR::Core PROPERTIES	INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include")	
set_target_properties(RTTR::Core PROPERTIES IMPORTED_LOCATION_DEBUG   "${install_dir}/lib/rttr_core_d${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_target_properties(RTTR::Core PROPERTIES IMPORTED_LOCATION_RELEASE "${install_dir}/lib/rttr_core${CMAKE_STATIC_LIBRARY_SUFFIX}")