#pragma once

#include <string>
#include <utility>

struct GLFWwindow;

namespace hzl
{
    struct WindowProperties
    {
        std::string title = "HZL Atom Simulation";
        int width = 1280;
        int height = 720;
    };

    class Window
    {
    public:
        explicit Window(WindowProperties properties);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void onUpdate();

        bool shouldClose() const;
        const WindowProperties& properties() const;
        bool isLeftMouseButtonPressed() const;
        std::pair<double, double> cursorPosition() const;
        double consumeScrollDeltaY();
        GLFWwindow* nativeHandle() const;

    private:
        static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

    private:
        WindowProperties m_properties;
        GLFWwindow* m_handle = nullptr;
        double m_scrollDeltaY = 0.0;
    };
}
