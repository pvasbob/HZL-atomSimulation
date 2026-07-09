#include "hzl/platform/Window.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <utility>

namespace hzl
{
    Window::Window(WindowProperties properties)
        : m_properties(std::move(properties))
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_handle = glfwCreateWindow(
            m_properties.width,
            m_properties.height,
            m_properties.title.c_str(),
            nullptr,
            nullptr);

        if (m_handle == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window.");
        }

        glfwMakeContextCurrent(m_handle);
        glfwSwapInterval(1);

        std::cout << "Window created: " << m_properties.title
                  << " (" << m_properties.width
                  << "x" << m_properties.height << ")\n";
    }

    Window::~Window()
    {
        if (m_handle != nullptr)
        {
            glfwDestroyWindow(m_handle);
        }

        glfwTerminate();
    }

    void Window::onUpdate()
    {
        glfwSwapBuffers(m_handle);
        glfwPollEvents();
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_handle);
    }

    const WindowProperties& Window::properties() const
    {
        return m_properties;
    }
}
