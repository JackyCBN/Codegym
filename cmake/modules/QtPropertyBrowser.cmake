include(ExternalProject)

set(configuration ${CMAKE_BUILD_TYPE})
ExternalProject_Add(QtPropertyBrowser_project
    PREFIX QtPropertyBrowser
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/externals/Qt5/QtPropertyBrowser"
	UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${configuration} -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_EXAMPLES=OFF --config ${configuration}
)

ExternalProject_Get_Property(QtPropertyBrowser_project install_dir)
set(QtPropertyBrowser_install_dir ${install_dir})
file(MAKE_DIRECTORY "${QtPropertyBrowser_install_dir}/include")
file(MAKE_DIRECTORY "${QtPropertyBrowser_install_dir}/lib")

set(rttr_debug_output_lib_path ${QtPropertyBrowser_install_dir}/lib/QtPropertyBrowserd${CMAKE_STATIC_LIBRARY_SUFFIX})
set(rttr_release_output_lib_path ${QtPropertyBrowser_install_dir}/lib/QtPropertyBrowser${CMAKE_STATIC_LIBRARY_SUFFIX})
# fails on Win: execute_process(COMMAND touch "${QtPropertyBrowser_install_dir}/lib/librttr_core.a")
# CMake >=3.12: file(TOUCH "${QtPropertyBrowser_install_dir}/lib/librttr_core.a")
if(NOT EXISTS "${rttr_debug_output_lib_path}")
     file(WRITE "${rttr_debug_output_lib_path}" "dummy debug file to be replaced by build")
endif()

if(NOT EXISTS "${rttr_release_output_lib_path}")
     file(WRITE "${rttr_release_output_lib_path}" "dummy release file to be replaced by build")
endif()

include(QtPropertyBrowser/QtPropertyBrowserTargets)

add_dependencies(QtPropertyBrowser QtPropertyBrowser_project)