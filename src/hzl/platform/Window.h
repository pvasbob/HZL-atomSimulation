#pragma once

#include <string>

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

        void onUpdate();

        bool shouldClose() const;
        const WindowProperties& properties() const;

    private:
        WindowProperties m_properties;
        int m_updateCount = 0;
    };
}
