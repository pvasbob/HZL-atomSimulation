#include "hzl/platform/Window.h"

#include <iostream>

namespace hzl
{
    Window::Window(WindowProperties properties)
        : m_properties(std::move(properties))
    {
        std::cout << "Window created: " << m_properties.title
                  << " (" << m_properties.width
                  << "x" << m_properties.height << ")\n";
    }

    void Window::onUpdate()
    {
        ++m_updateCount;
    }

    bool Window::shouldClose() const
    {
        return m_updateCount >= 5;
    }

    const WindowProperties& Window::properties() const
    {
        return m_properties;
    }
}
