
#include "Window.h"

namespace codegym::runtime
{
    Window* (*Window::CreateFunc)(const WindowDesc&) = NULL;

    Window::~Window()
    {

    }

    bool Window::Initialize(const WindowDesc& windowDesc)
    {
        return HasInitialized();
    }
}
