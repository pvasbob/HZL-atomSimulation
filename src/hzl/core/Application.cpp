#include "hzl/core/Application.h"

#include <iostream>

namespace hzl
{
    Application::Application()
    {
        std::cout << "Application created.\n";
    }

    void Application::run()
    {
        std::cout << "HZL Atom Simulation running...\n";

        while (m_running)
        {
            update();
            render();

            ++m_frameIndex;

            if (m_frameIndex >= 5)
            {
                m_running = false;
            }
        }

        std::cout << "Application shutting down.\n";
    }

    void Application::update()
    {
        std::cout << "Frame " << m_frameIndex << ": update simulation\n";
    }

    void Application::render()
    {
        std::cout << "Frame " << m_frameIndex << ": render frame\n";
    }
}
