#include "hzl/core/Application.h"

#include <chrono>
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

        using Clock = std::chrono::steady_clock;
        auto lastFrameTime = Clock::now();

        while (m_running)
        {
            const auto currentFrameTime = Clock::now();
            const std::chrono::duration<float> elapsedTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            const Timestep timestep(elapsedTime.count());

            update(timestep);
            render();

            ++m_frameIndex;

            if (m_frameIndex >= 5)
            {
                m_running = false;
            }
        }

        std::cout << "Application shutting down.\n";
    }

    void Application::update(Timestep timestep)
    {
        std::cout << "Frame " << m_frameIndex
                  << ": update simulation, dt = "
                  << timestep.milliseconds() << " ms\n";
    }

    void Application::render()
    {
        std::cout << "Frame " << m_frameIndex << ": render frame\n";
    }
}
