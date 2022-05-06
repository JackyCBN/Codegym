#pragma once
#include "Base.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Core/OS/KeyCodes.h"
#include "Math/Vector2.h"

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

namespace codegym::runtime {
    class Event;

    enum class MouseMode
    {
        Visible,
        Hidden,
        Captured
    };

    class CG_API Input 
    {

    public:
      static Input& Get() {
        static Input inst;
        return inst;
      }
        Input();
        virtual ~Input() = default;

        bool GetKeyPressed(InputCode::Key key) const { return m_KeyPressed[int(key)]; }
        bool GetKeyHeld(InputCode::Key key) const { return m_KeyHold[int(key)]; }
        bool GetMouseClicked(InputCode::MouseKey key) const { return m_MouseClicked[int(key)]; }
        bool GetMouseHeld(InputCode::MouseKey key) const { return m_MouseHold[int(key)]; }

        void SetKeyPressed(InputCode::Key key, bool a) { m_KeyPressed[int(key)] = a; }
        void SetKeyHeld(InputCode::Key key, bool a) { m_KeyHold[int(key)] = a; }
        void SetMouseClicked(InputCode::MouseKey key, bool a) { m_MouseClicked[int(key)] = a; }
        void SetMouseHeld(InputCode::MouseKey key, bool a) { m_MouseHold[int(key)] = a; }

        void SetMouseOnScreen(bool onScreen) { m_MouseOnScreen = onScreen; }
        bool GetMouseOnScreen() const { return m_MouseOnScreen; }

        void StoreMousePosition(float xpos, float ypos) { m_MousePosition = Vector2(float(xpos), float(ypos)); }
        const Vector2& GetMousePosition() const { return m_MousePosition; }

        void SetScrollOffset(float offset) { m_ScrollOffset = offset; }
        float GetScrollOffset() const { return m_ScrollOffset; }

        void Reset();
        void ResetPressed();
        void OnEvent(Event& e);

        MouseMode GetMouseMode() const { return m_MouseMode; }
        void SetMouseMode(MouseMode mode) { m_MouseMode = mode; }

    protected:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnMousePressed(MouseButtonPressedEvent& e);
        bool OnMouseReleased(MouseButtonReleasedEvent& e);
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnMouseEnter(MouseEnterEvent& e);

        bool m_KeyPressed[MAX_KEYS];
        bool m_KeyHold[MAX_KEYS];

        bool m_MouseHold[MAX_BUTTONS];
        bool m_MouseClicked[MAX_BUTTONS];

        float m_ScrollOffset = 0.0f;

        bool m_MouseOnScreen;
        MouseMode m_MouseMode;

        Vector2 m_MousePosition;
    };
}
