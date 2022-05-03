
#include "GLFWWindow.h"
#include "GLFWKeyCodes.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Logging/LogAssert.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#  include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers


namespace codegym::runtime
{
    static int s_NumcgGLFWWindows = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        LOG_ERROR("GLFW Error - {0} : {1}", error, description);
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    Window* cgGLFWWindow::Create(const WindowDesc& windowDesc) {
      return CreateFuncGLFW(windowDesc);
    }

    cgGLFWWindow::cgGLFWWindow(const WindowDesc& properties)
    {
        PROFILE_FUNCTION();
        m_Init = false;
        m_VSync = properties.VSync;

        LOG_INFO("VSync : {0}", m_VSync ? "True" : "False");
        m_Data.VSync = m_VSync;
        m_Init = Init(properties);
    }

    cgGLFWWindow::~cgGLFWWindow()
    {
        glfwDestroyWindow(m_Handle);

        if(--s_NumcgGLFWWindows ==0)
        {
            glfwTerminate();
        }
    }

    void cgGLFWWindow::registerWindowEvent() {
      // Set GLFW callbacks
      glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int width, int height)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        data.DPIScale = (float)w / (float)width;

        data.Width = uint32_t(width * data.DPIScale);
        data.Height = uint32_t(height * data.DPIScale);

        WindowResizeEvent event(data.Width, data.Height, data.DPIScale);
        data.EventCallback(event);
      });

      glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));
        WindowCloseEvent event;
        data.EventCallback(event);
        data.Exit = true;
      });

      glfwSetWindowFocusCallback(m_Handle, [](GLFWwindow* window, int focused) {
        //Application::Get().GetWindow()->SetWindowFocus(focused);
      });

      glfwSetWindowIconifyCallback(m_Handle, [](GLFWwindow* window, int32_t state)
      {
        switch(state)
        {
          case GL_TRUE:
            //Application::Get().GetWindow()->SetWindowFocus(false);
            break;
          case GL_FALSE:
            //Application::Get().GetWindow()->SetWindowFocus(true);
            break;
          default:
            LOG_INFO("Unsupported window iconify state from callback");
        }
      });

      glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));

        switch(action)
        {
          case GLFW_PRESS:
          {
            KeyPressedEvent event(GLFWKeyCodes::GLFWToLumosKeyboardKey(key), 0);
            data.EventCallback(event);
            break;
          }
          case GLFW_RELEASE:
          {
            KeyReleasedEvent event(GLFWKeyCodes::GLFWToLumosKeyboardKey(key));
            data.EventCallback(event);
            break;
          }
          case GLFW_REPEAT:
          {
            KeyPressedEvent event(GLFWKeyCodes::GLFWToLumosKeyboardKey(key), 1);
            data.EventCallback(event);
            break;
          }
        }
      });

      glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* window, int button, int action, int mods)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));

        switch(action)
        {
          case GLFW_PRESS:
          {
            MouseButtonPressedEvent event(GLFWKeyCodes::GLFWToLumosMouseKey(button));
            data.EventCallback(event);
            break;
          }
          case GLFW_RELEASE:
          {
            MouseButtonReleasedEvent event(GLFWKeyCodes::GLFWToLumosMouseKey(button));
            data.EventCallback(event);
            break;
          }
        }
      });

      glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xOffset, double yOffset)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));
        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
      });

      glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* window, double xPos, double yPos)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));
        MouseMovedEvent event((float)xPos /* * data.DPIScale*/, (float)yPos /* * data.DPIScale*/);
        data.EventCallback(event);
      });

      glfwSetCursorEnterCallback(m_Handle, [](GLFWwindow* window, int enter)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));

        MouseEnterEvent event(enter > 0);
        data.EventCallback(event);
      });

      glfwSetCharCallback(m_Handle, [](GLFWwindow* window, unsigned int keycode)
      {
        WindowData& data = *static_cast<WindowData*>((glfwGetWindowUserPointer(window)));

        KeyTypedEvent event(GLFWKeyCodes::GLFWToLumosKeyboardKey(keycode), char(keycode));
        data.EventCallback(event);
      });
    }

    bool cgGLFWWindow::Init(const WindowDesc& properties)
    {
        PROFILE_FUNCTION();
        LOG_INFO("Creating window - Title : {0}, Width : {1}, Height : {2}", properties.Title, properties.Width, properties.Height);

        if (s_NumcgGLFWWindows++==0)
        {
            int success = glfwInit();
            ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
        

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        float xscale, yscale;
        glfwGetMonitorContentScale(monitor, &xscale, &yscale);
        m_Data.DPIScale = xscale;


        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        uint32_t ScreenWidth = 0;
        uint32_t ScreenHeight = 0;

        if(properties.Fullscreen)
        {
            ScreenWidth = mode->width;
            ScreenHeight = mode->height;
        }
        else
        {
            ScreenWidth = properties.Width;
            ScreenHeight = properties.Height;
        }

        m_Data.Title = properties.Title;
        m_Data.Width = ScreenWidth;
        m_Data.Height = ScreenHeight;
        m_Data.Exit = false;

        m_Handle = glfwCreateWindow(ScreenWidth, ScreenHeight, properties.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Handle);
        int w, h;
        glfwGetFramebufferSize(m_Handle, &w, &h);
        m_Data.Width = w;
        m_Data.Height = h;
        

        glfwSetWindowUserPointer(m_Handle, &m_Data);

        if(glfwRawMouseMotionSupported())
            glfwSetInputMode(m_Handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        //glfwSetWindowPos(m_Handle, mode->width / 2 - m_Data.Width / 2, mode->height / 2 - m_Data.Height / 2);
        glfwSetInputMode(m_Handle, GLFW_STICKY_KEYS, true);


        registerWindowEvent();
        
        LOG_INFO("Initialised GLFW version : {0}", glfwGetVersionString());

        return true;
    }

    void cgGLFWWindow::SetWindowTitle(const std::string& title)
    {
        PROFILE_FUNCTION();
        glfwSetWindowTitle(m_Handle, title.c_str());
    }

    void cgGLFWWindow::ToggleVSync()
    {
        PROFILE_FUNCTION();
        if(m_VSync)
        {
            SetVSync(false);
        }
        else
        {
            SetVSync(true);
        }

        LOG_INFO("VSync : {0}", m_VSync ? "True" : "False");
    }

    void cgGLFWWindow::SetVSync(bool set)
    {
        PROFILE_FUNCTION();
        if(set)
        {
            m_VSync = true;
            glfwSwapInterval(1);
        }
        else
        {
            m_VSync = false;
            glfwSwapInterval(0);
        }
        LOG_INFO("VSync : {0}", m_VSync ? "True" : "False");
    }

    void cgGLFWWindow::OnUpdate()
    {
        PROFILE_FUNCTION();
    }

    void cgGLFWWindow::HideMouse(bool hide)
    {
        PROFILE_FUNCTION();
        if(hide)
        {
            glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    void cgGLFWWindow::SetMousePosition(const Vector2& pos)
    {
        PROFILE_FUNCTION();
        //Input::Get().StoreMousePosition(pos.x, pos.y);
        glfwSetCursorPos(m_Handle, pos.x, pos.y);
    }

    void cgGLFWWindow::MakeDefault()
    {
        CreateFunc = CreateFuncGLFW;
    }

    Window* cgGLFWWindow::CreateFuncGLFW(const WindowDesc& properties)
    {
        return new cgGLFWWindow(properties);
    }

    void cgGLFWWindow::UpdateCursorImGui()
    {
        PROFILE_FUNCTION();
    }

    void cgGLFWWindow::ProcessInput()
    {
        PROFILE_SCOPE("GLFW PollEvents");
        glfwPollEvents();
    }
}
