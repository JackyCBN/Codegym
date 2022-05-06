//#include "Precompiled.h"
#include "Application.h"

#include <backends/imgui_impl_opengl3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"
#include "OS/Window.h"

#include "Logging/LogAssert.h"
#include "OS/Input.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/GLFW/GLFWWindow.h"

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

      // Setup Dear ImGui context
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      (void)io;
      // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
      // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

      // Setup Dear ImGui style
      ImGui::StyleColorsDark();
      // ImGui::StyleColorsClassic();
      const char* glsl_version = "#version 150";
      // Setup Platform/Renderer backends
      ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)(GetWindow()->GetHandle()), true);
      ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void Application::OnQuit()
    {
        PROFILE_FUNCTION();

      // Cleanup
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    }

    bool Application::OnFrame()
    {
        OnUpdate();
        OnRender();
        return !m_Window->GetExit();
    }

    void Application::OnRender()
    {
      // Our state
      bool show_demo_window = true;
      bool show_another_window = false;
      ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      GLFWwindow* window = (GLFWwindow*)(m_Window->GetHandle());
      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You
      // can browse its code to learn more about Dear ImGui!).
      if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

      // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a
      // named window.
      {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin(
            "Hello, world!");  // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");  // Display some text (you can use a format
                                                   // strings too)
        ImGui::Checkbox("Demo Window",
                        &show_demo_window);  // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f,
                           1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color",
                          (float*)&clear_color);  // Edit 3 floats representing a color

        if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return
                                      // true when edited/activated)
          counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
      }

      // 3. Show another simple window.
      if (show_another_window) {
        ImGui::Begin(
            "Another Window",
            &show_another_window);  // Pass a pointer to our bool variable (the window will have
                                    // a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) show_another_window = false;
        ImGui::End();
      }

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                   clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
    }
    
    void Application::OnUpdate()
    {
      Input::Get().ResetPressed();
      m_Window->ProcessInput();

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
