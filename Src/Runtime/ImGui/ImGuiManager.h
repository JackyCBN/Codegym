#pragma once

#include "Base.h"
#include "Events/KeyEvent.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace codegym::runtime {
    class TimeStep;

    class CG_API ImGuiManager
    {
    public:
        ImGuiManager();
        ~ImGuiManager();

        void OnInit();
        void OnUpdate(const TimeStep& dt);
        void OnEvent(Event& event);
        void OnRender();

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnwindowResizeEvent(WindowResizeEvent& e);

        void SetImGuiKeyCodes();
        void SetImGuiStyle();

        float m_FontSize;
        float m_DPIScale;
    };

}
