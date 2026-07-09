#pragma once

#include <string>

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

    private:
        WindowProperties m_properties;
        GLFWwindow* m_handle = nullptr;
    };
}
