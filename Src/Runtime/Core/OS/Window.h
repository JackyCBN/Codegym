#pragma once
#include "Base.h"
#include "Math/Vector2.h"
#include "Events/Event.h"
namespace codegym::runtime
{
    struct CG_API WindowDesc
    {
        WindowDesc(uint32_t width = 1280, uint32_t height = 720, int renderAPI = 0, const std::string& title = "Lumos", bool fullscreen = false, bool vSync = true, bool borderless = false, const std::string& filepath = "")
            : Width(width)
            , Height(height)
            , Title(title)
            , Fullscreen(fullscreen)
            , VSync(vSync)
        {
        }

        uint32_t Width, Height;
        bool Fullscreen;
        bool VSync;
        bool ShowConsole = true;
        std::string Title;
    };

    class CG_API Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window();
        bool Initialize(const WindowDesc& windowDesc);

        bool HasInitialized() const
        {
            return m_Init;
        };

        virtual bool GetExit() const = 0;
        virtual void SetExit(bool exit) = 0;

        virtual void ToggleVSync() = 0;
        virtual void SetVSync(bool set) = 0;
        virtual void SetWindowTitle(const std::string& title) = 0;
        virtual void OnUpdate() = 0;
        virtual void ProcessInput() {};
        virtual void* GetHandle()
        {
            return nullptr;
        };
        virtual float GetScreenRatio() const = 0;
        virtual void HideMouse(bool hide) {};
        virtual void SetMousePosition(const Vector2& pos) {};
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void UpdateCursorImGui() = 0;
        virtual std::string GetTitle() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual float GetDPIScale() const { return 1.0f; }

        void SetWindowFocus(bool focus) { m_WindowFocus = focus; }
        bool GetWindowFocus() const { return m_WindowFocus; }
        virtual void OnFocusChanged(bool bFocus) {}
    protected:

        static Window* (*CreateFunc)(const WindowDesc&);

        Window() = default;

        bool m_Init = false;
        Vector2 m_Position;
        bool m_VSync = false;
        bool m_WindowFocus = true;
    };

}
