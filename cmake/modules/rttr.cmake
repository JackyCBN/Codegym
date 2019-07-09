

include(ExternalProject)

set(configuration ${CMAKE_BUILD_TYPE})
ExternalProject_Add(rttr_project
    PREFIX rttr
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/rttr"
	UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${configuration} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_INSTALL_LIBDIR=lib -DBUILD_EXAMPLES=OFF -DBUILD_DOCUMENTATION=OFF -DBUILD_UNIT_TESTS=OFF -DBUILD_PACKAGE=OFF -DBUILD_STATIC=ON --target install  --config ${configuration}
)

ExternalProject_Get_Property(rttr_project install_dir)
file(MAKE_DIRECTORY "${install_dir}/include")
file(MAKE_DIRECTORY "${install_dir}/lib")
# fails on Win: execute_process(COMMAND touch "${install_dir}/lib/librttr_core.a")
# CMake >=3.12: file(TOUCH "${install_dir}/lib/librttr_core.a")
#if(NOT EXISTS "${install_dir}/lib/librttr_core.a")
    # file(WRITE "${install_dir}/lib/librttr_core.a" "dummy file to be replaced by build")
#endif()

add_library(RTTR::Core_Lib STATIC IMPORTED)
add_dependencies(RTTR::Core_Lib rttr_project)
set_target_properties(RTTR::Core_Lib PROPERTIES	INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include")	
set_target_properties(RTTR::Core_Lib PROPERTIES IMPORTED_LOCATION_DEBUG "${install_dir}/lib/librttr_core_d${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_target_properties(RTTR::Core_Lib PROPERTIES IMPORTED_LOCATION_RELEASE "${install_dir}/lib/librttr_core${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_target_properties(RTTR::Core_Lib PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO "${install_dir}/lib/librttr_core${CMAKE_STATIC_LIBRARY_SUFFIX}")