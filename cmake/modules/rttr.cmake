include(ExternalProject)

set(configuration ${CMAKE_BUILD_TYPE})
ExternalProject_Add(rttr_project
    PREFIX rttr
	SOURCE_DIR "${CMAKE_SOURCE_DIR}/Externals/rttr"
	UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${configuration} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_EXAMPLES=OFF -DBUILD_DOCUMENTATION=OFF -DBUILD_UNIT_TESTS=OFF -DBUILD_PACKAGE=OFF -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY} --config ${configuration}
)

ExternalProject_Get_Property(rttr_project install_dir)
set(rttr_install_dir ${install_dir})
file(MAKE_DIRECTORY "${rttr_install_dir}/include")
file(MAKE_DIRECTORY "${rttr_install_dir}/lib")

set(rttr_debug_output_lib_path ${rttr_install_dir}/lib/rttr_core_d${CMAKE_STATIC_LIBRARY_SUFFIX})
set(rttr_release_output_lib_path ${rttr_install_dir}/lib/rttr_core${CMAKE_STATIC_LIBRARY_SUFFIX})
# fails on Win: execute_process(COMMAND touch "${rttr_install_dir}/lib/librttr_core.a")
# CMake >=3.12: file(TOUCH "${rttr_install_dir}/lib/librttr_core.a")
if(NOT EXISTS "${rttr_debug_output_lib_path}")
     file(WRITE "${rttr_debug_output_lib_path}" "dummy debug file to be replaced by build")
endif()

if(NOT EXISTS "${rttr_release_output_lib_path}")
     file(WRITE "${rttr_release_output_lib_path}" "dummy release file to be replaced by build")
endif()

include(rttr/rttr-config)

add_dependencies(RTTR::Core rttr_project)