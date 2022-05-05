#include <rttr/type>
#include <rttr/registration>
#include "Runtime/ExportTest.h"
#include "Menu/MenuManager.h"
#include "Scene/SceneGraph.h"
#include "Manager/ManagerMgr.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#  include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "Core/Application.h"
#include "Core/OS/Window.h"
#include "Platform/GLFW/GLFWWindow.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of
// testing and compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+
// requires linking with legacy_stdio_definitions.lib, which we do using this pragma. Your own
// project should not be affected, as you are likely to link with a newer binary of GLFW that is
// adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#  pragma comment(lib, "legacy_stdio_definitions")
#endif

using namespace codegym::editor;
using namespace codegym::runtime;
static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int WinMain(int argc, char **argv)
{
	ExportTest et;

	auto mgr = make_unique<ManagerMgr>();
	mgr->Initlize();
	SceneGraph  sg;
	sg.Initlize();

        Application app;
        app.Init();
        app.Run();

	/*QApplication app (argc, argv);
	MainWindow mw;
	
	cMenuManager::Initlize(&mw);
	ExecuteStartups();
	cMenuManager::RebuildOsMenus();
	mw.SetupLayout();
	mw.InitContent(&sg);
	mw.showMaximized();

	return app.exec();*/
        // Setup window

        return 0;
}
