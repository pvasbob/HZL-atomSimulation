#include "hzl/platform/Window.h"

#include <glad/gl.h>
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

        glfwSetWindowUserPointer(m_handle, this);
        glfwSetScrollCallback(m_handle, &Window::scrollCallback);

        glfwMakeContextCurrent(m_handle);
        glfwSwapInterval(1);

        const int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0)
        {
            glfwDestroyWindow(m_handle);
            m_handle = nullptr;
            glfwTerminate();
            throw std::runtime_error("Failed to load OpenGL functions with GLAD.");
        }

        std::cout << "Window created: " << m_properties.title
                  << " (" << m_properties.width
                  << "x" << m_properties.height << ")\n";
        std::cout << "Loaded OpenGL "
                  << GLAD_VERSION_MAJOR(version) << "."
                  << GLAD_VERSION_MINOR(version) << "\n";
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

    bool Window::isLeftMouseButtonPressed() const
    {
        return glfwGetMouseButton(m_handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    std::pair<double, double> Window::cursorPosition() const
    {
        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(m_handle, &x, &y);

        return {x, y};
    }

    double Window::consumeScrollDeltaY()
    {
        const double scrollDeltaY = m_scrollDeltaY;
        m_scrollDeltaY = 0.0;

        return scrollDeltaY;
    }

    GLFWwindow* Window::nativeHandle() const
    {
        return m_handle;
    }

    void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        (void)xOffset;

        auto* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (currentWindow == nullptr)
        {
            return;
        }

        currentWindow->m_scrollDeltaY += yOffset;
    }
}
