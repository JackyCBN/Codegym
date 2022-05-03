#pragma once

#include "Core/OS/Window.h"
#include "Math/Vector2.h"
struct GLFWwindow;

namespace codegym::runtime
{
    class CG_API cgGLFWWindow : public Window
    {
    public:

        cgGLFWWindow(const WindowDesc& properties);
        ~cgGLFWWindow();

        void ToggleVSync() override;
        void SetVSync(bool set) override;
        void SetWindowTitle(const std::string& title) override;
        void OnUpdate() override;
        void HideMouse(bool hide) override;
        void SetMousePosition(const Vector2& pos) override;
        void UpdateCursorImGui() override;
        void ProcessInput() override;

        bool Init(const WindowDesc& properties);

        inline void* GetHandle() override
        {
            return m_Handle;
        }

        inline std::string GetTitle() const override
        {
            return m_Data.Title;
        }
        inline uint32_t GetWidth() const override
        {
            return m_Data.Width;
        }
        inline uint32_t GetHeight() const override
        {
            return m_Data.Height;
        }

        inline float GetDPIScale() const override
        {
            return m_Data.DPIScale;
        }

        inline float GetScreenRatio() const override
        {
            return (float)m_Data.Width / (float)m_Data.Height;
        }

        inline bool GetExit() const override
        {
            return m_Data.Exit;
        }

        inline void SetExit(bool exit) override
        {
            m_Data.Exit = exit;
        }

        inline void SetEventCallback(const EventCallbackFn& callback) override
        {
            m_Data.EventCallback = callback;
        }

        static Window* Create(const WindowDesc& windowDesc);
        static void MakeDefault();

    protected:
        static Window* CreateFuncGLFW(const WindowDesc& properties);
        void registerWindowEvent();
        GLFWwindow* m_Handle;

        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
            bool Exit;
            float DPIScale;

            EventCallbackFn EventCallback = [](Event&) {
              
            };
        };

        WindowData m_Data;
    };
}
