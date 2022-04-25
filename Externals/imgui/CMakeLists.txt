CPMAddPackage("gh:ocornut/imgui#docking")

set(libsrc 
${imgui_SOURCE_DIR}/imgui_internal.h
${imgui_SOURCE_DIR}/imconfig.h
${imgui_SOURCE_DIR}/imgui.cpp
${imgui_SOURCE_DIR}/imgui.h
${imgui_SOURCE_DIR}/imgui_demo.cpp
${imgui_SOURCE_DIR}/imgui_draw.cpp
${imgui_SOURCE_DIR}/imgui_tables.cpp
${imgui_SOURCE_DIR}/imgui_widgets.cpp
${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

#[[
imgui_user/imgui_user.cpp
imgui_user/imgui_user.h
imgui_user/imgui_date_picker.cpp
imgui_user/imgui_date_picker.h
imguizmo/imguizmo.cpp
imguizmo/imguizmo.h
]]
)

add_library (imgui STATIC ${libsrc})

if(MSVC)
set(imgui_flags
_CRT_SECURE_NO_WARNINGS
_CRT_SECURE_NO_DEPRECATE)
endif()

set(imgui_flags ${imgui_flags}
IMGUI_DISABLE_OBSOLETE_FUNCTIONS)

target_compile_definitions(imgui PUBLIC ${imgui_flags})
#target_include_directories (imgui PUBLIC ${imgui_SOURCE_DIR})

target_link_libraries(imgui PUBLIC glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARIES})

target_include_directories(imgui PUBLIC
                               $<BUILD_INTERFACE:${imgui_BINARY_DIR}>
                               $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
				                $<BUILD_INTERFACE:${ConfigurationDirectory}>
                               $<INSTALL_INTERFACE:include>)
