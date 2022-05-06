#include "ImGuiManager.h"

#include <backends/imgui_impl_opengl3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "Core/OS/Input.h"
#include "Core/OS/Window.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

namespace codegym::runtime {
    ImGuiManager::ImGuiManager()
    {
        m_FontSize = 14.0f;
    }

    ImGuiManager::~ImGuiManager()
    {
      // Cleanup
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    }
    
    static const char* ImGui_ImplGlfw_GetClipboardText(void*)
    {
        return glfwGetClipboardString((GLFWwindow*)Application::Get().GetWindow()->GetHandle());
    }

    static void ImGui_ImplGlfw_SetClipboardText(void*, const char* text)
    {
        glfwSetClipboardString((GLFWwindow*)Application::Get().GetWindow()->GetHandle(), text);
    }

    void ImGuiManager::OnInit()
    {
        PROFILE_FUNCTION();

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
      Application& app = Application::Get();
      ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)(app.GetWindow()->GetHandle()), true);
      ImGui_ImplOpenGL3_Init(glsl_version);


        LOG_INFO("ImGui Version : {0}", IMGUI_VERSION);
#ifdef IMGUI_USER_CONFIG
        LOG_INFO("ImConfig File : {0}", std::string(IMGUI_USER_CONFIG));
#endif
        io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow()->GetWidth()), static_cast<float>(app.GetWindow()->GetHeight()));
        //io.DisplayFramebufferScale = ImVec2(app.GetWindow()->GetDPIScale(), app.GetWindow()->GetDPIScale());
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        m_DPIScale = app.GetWindow()->GetDPIScale();
#ifdef LUMOS_PLATFORM_IOS
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
#endif
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.ConfigWindowsMoveFromTitleBarOnly = true;

        m_FontSize *= app.GetWindow()->GetDPIScale();

       // SetImGuiKeyCodes();
        SetImGuiStyle();

        io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
        io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
    }

    void ImGuiManager::OnUpdate(const TimeStep& dt)
    {
        PROFILE_FUNCTION();
    }

    void ImGuiManager::OnEvent(Event& event)
    {
        PROFILE_FUNCTION();
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiManager::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiManager::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiManager::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiManager::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiManager::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiManager::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiManager::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiManager::OnwindowResizeEvent));
    }

    void ImGuiManager::OnRender()
    {
        PROFILE_FUNCTION();

      // Our state
      bool show_demo_window = true;
      bool show_another_window = false;
      ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      GLFWwindow* window = (GLFWwindow*)(Application::Get().GetWindow()->GetHandle());
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
    

    int LumosMouseButtonToImGui(InputCode::MouseKey key)
    {
        switch(key)
        {
        case InputCode::MouseKey::ButtonLeft:
            return 0;
        case InputCode::MouseKey::ButtonRight:
            return 1;
        case InputCode::MouseKey::ButtonMiddle:
            return 2;
        default:
            return 4;
        }

        return 4;
    }

    bool ImGuiManager::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[LumosMouseButtonToImGui(e.GetMouseButton())] = true;

        return false;
    }

    bool ImGuiManager::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[LumosMouseButtonToImGui(e.GetMouseButton())] = false;

        return false;
    }

    bool ImGuiManager::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        if(Input::Get().GetMouseMode() == MouseMode::Visible)
            io.MousePos = ImVec2(e.GetX() * m_DPIScale, e.GetY() * m_DPIScale);

        return false;
    }

    bool ImGuiManager::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.GetYOffset() / 10.0f;
        io.MouseWheelH += e.GetXOffset() / 10.0f;

        return false;
    }

    bool ImGuiManager::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[(int)e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[(int)InputCode::Key::LeftControl] || io.KeysDown[(int)InputCode::Key::RightControl];
        io.KeyShift = io.KeysDown[(int)InputCode::Key::LeftShift] || io.KeysDown[(int)InputCode::Key::RightShift];
        io.KeyAlt = io.KeysDown[(int)InputCode::Key::LeftAlt] || io.KeysDown[(int)InputCode::Key::RightAlt];

#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[(int)InputCode::Key::LeftSuper] || io.KeysDown[(int)InputCode::Key::RightSuper];
#endif

        return io.WantTextInput;
    }

    bool ImGuiManager::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[(int)e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiManager::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = (int)e.Character;
        if(keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

    bool ImGuiManager::OnwindowResizeEvent(WindowResizeEvent& e)
    {
        PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();

        uint32_t width = std::max(1u, e.GetWidth());
        uint32_t height = std::max(1u, e.GetHeight());

        io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
        //io.DisplayFramebufferScale = ImVec2(e.GetDPIScale(), e.GetDPIScale());
        m_DPIScale = e.GetDPIScale();

        return false;
    }

    void ImGuiManager::SetImGuiKeyCodes()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.KeyMap[ImGuiKey_Tab] = (int)InputCode::Key::Tab;
        io.KeyMap[ImGuiKey_LeftArrow] = (int)InputCode::Key::Left;
        io.KeyMap[ImGuiKey_RightArrow] = (int)InputCode::Key::Right;
        io.KeyMap[ImGuiKey_UpArrow] = (int)InputCode::Key::Up;
        io.KeyMap[ImGuiKey_DownArrow] = (int)InputCode::Key::Down;
        io.KeyMap[ImGuiKey_PageUp] = (int)InputCode::Key::PageUp;
        io.KeyMap[ImGuiKey_PageDown] = (int)InputCode::Key::PageDown;
        io.KeyMap[ImGuiKey_Home] = (int)InputCode::Key::Home;
        io.KeyMap[ImGuiKey_End] = (int)InputCode::Key::End;
        io.KeyMap[ImGuiKey_Insert] = (int)InputCode::Key::Insert;
        io.KeyMap[ImGuiKey_Delete] = (int)InputCode::Key::Delete;
        io.KeyMap[ImGuiKey_Backspace] = (int)InputCode::Key::Backspace;
        io.KeyMap[ImGuiKey_Space] = (int)InputCode::Key::Space;
        io.KeyMap[ImGuiKey_Enter] = (int)InputCode::Key::Enter;
        io.KeyMap[ImGuiKey_Escape] = (int)InputCode::Key::Escape;
        io.KeyMap[ImGuiKey_A] = (int)InputCode::Key::A;
        io.KeyMap[ImGuiKey_C] = (int)InputCode::Key::C;
        io.KeyMap[ImGuiKey_V] = (int)InputCode::Key::V;
        io.KeyMap[ImGuiKey_X] = (int)InputCode::Key::X;
        io.KeyMap[ImGuiKey_Y] = (int)InputCode::Key::Y;
        io.KeyMap[ImGuiKey_Z] = (int)InputCode::Key::Z;
        io.KeyRepeatDelay = 0.400f;
        io.KeyRepeatRate = 0.05f;
    }

    void ImGuiManager::SetImGuiStyle()
    {
        PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();

        ImGui::StyleColorsDark();

        io.FontGlobalScale = 1.0f;

        ImFontConfig icons_config;
        icons_config.MergeMode = false;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = icons_config.OversampleV = 1;
        icons_config.GlyphMinAdvanceX = 4.0f;
        icons_config.SizePixels = 12.0f;

        static const ImWchar ranges[] = {
            0x0020,
            0x00FF,
            0x0400,
            0x044F,
            0,
        };

        io.Fonts->AddFontDefault();

        io.Fonts->TexGlyphPadding = 1;
        for(int n = 0; n < io.Fonts->ConfigData.Size; n++)
        {
            ImFontConfig* font_config = (ImFontConfig*)&io.Fonts->ConfigData[n];
            font_config->RasterizerMultiply = 1.0f;
        }

        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowPadding = ImVec2(5, 5);
        style.FramePadding = ImVec2(4, 4);
        style.ItemSpacing = ImVec2(6, 2);
        style.ItemInnerSpacing = ImVec2(2, 2);
        style.IndentSpacing = 6.0f;
        style.TouchExtraPadding = ImVec2(4, 4);

        style.ScrollbarSize = 10;

        style.WindowBorderSize = 0;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 3;
        style.FrameBorderSize = 0.0f;

        const int roundingAmount = 2;
        style.PopupRounding = roundingAmount;
        style.WindowRounding = roundingAmount;
        style.ChildRounding = 0;
        style.FrameRounding = roundingAmount;
        style.ScrollbarRounding = roundingAmount;
        style.GrabRounding = roundingAmount;
        style.WindowMinSize = ImVec2(200.0f, 200.0f);

#ifdef IMGUI_HAS_DOCK
        style.TabBorderSize = 1.0f;
        style.TabRounding = roundingAmount; // + 4;

        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = roundingAmount;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
#endif
    }
    
}
