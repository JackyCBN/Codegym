#pragma once

#include "Event.h"
#include "Core/OS/KeyCodes.h"

#include <sstream>

namespace codegym::runtime
{

    class CG_API KeyEvent : public Event
    {
    public:
      InputCode::Key GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(InputCode::Key keycode)
            : m_KeyCode(keycode)
        {
        }

        InputCode::Key m_KeyCode;
    };

    class CG_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(InputCode::Key keycode, int repeatCount)
            : KeyEvent(keycode)
            , m_RepeatCount(repeatCount)
        {
        }

        inline int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << uint32_t(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class CG_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(InputCode::Key keycode)
            : KeyEvent(keycode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << uint32_t(m_KeyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class CG_API KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(InputCode::Key keycode, char character)
            : KeyEvent(keycode)
            , Character(character)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << uint32_t(m_KeyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)

        char Character;

    private:
    };
}
