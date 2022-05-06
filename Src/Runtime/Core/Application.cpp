//#include "Precompiled.h"
#include "Application.h"

#include "imgui.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiManager.h"
#include "OS/Window.h"

#include "Logging/LogAssert.h"
#include "OS/Input.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Utilities/TimeStep.h"

namespace codegym::runtime {
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        PROFILE_FUNCTION();
        ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
    }

    Application::~Application()
    {
        PROFILE_FUNCTION();
    }
    
    void Application::Init()
    {
        PROFILE_FUNCTION();
      WindowDesc windowDesc;
      m_Window.reset(cgGLFWWindow::Create(windowDesc));
      m_Window->SetWindowTitle("Dear ImGui GLFW + OpenGL3 example");

      m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

      //  glfwMakeContextCurrent(window);
      // glfwSwapInterval(1);  // Enable vsync

      m_imguiManager.reset(new ImGuiManager());
      m_imguiManager->OnInit();
    }

    void Application::OnQuit()
    {
        PROFILE_FUNCTION();
      m_imguiManager.reset();
    }

    bool Application::OnFrame()
    {
        OnUpdate(TimeStep{0});
        OnRender();
        return !m_Window->GetExit();
    }

    void Application::OnRender()
    {
      m_imguiManager->OnRender();
    }
    
    void Application::OnUpdate(const TimeStep& dt)
    {
      Input::Get().ResetPressed();
      m_Window->ProcessInput();
      m_imguiManager->OnUpdate(dt);

      if (Input::Get().GetKeyPressed(InputCode::Key::Escape)) {
        m_Window->SetExit(true);
      }
    }

    Application* CreateApplication() {
      return new Application();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
      return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
      

      return false;
    }
    void Application::OnEvent(Event& e)
    {
        PROFILE_FUNCTION();
      EventDispatcher dispatcher(e);
      dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
      dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
      if (m_imguiManager) m_imguiManager->OnEvent(e);
      if (e.Handled()) {
        return ;
      }
      Input::Get().OnEvent(e);
    }

    void Application::Run()
    {
        while(OnFrame())
        {
        }

        OnQuit();
    }
}
