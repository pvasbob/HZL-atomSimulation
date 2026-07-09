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
        initialize();

        using Clock = std::chrono::steady_clock;
        auto lastFrameTime = Clock::now();

        while (m_running)
        {
            const auto currentFrameTime = Clock::now();
            const std::chrono::duration<float> elapsedTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            const Timestep timestep(elapsedTime.count());

            runFrame(timestep);

            ++m_frameIndex;

            if (shouldClose())
            {
                m_running = false;
            }
        }

        shutdown();
    }

    void Application::initialize()
    {
        std::cout << "HZL Atom Simulation starting up...\n";
    }

    void Application::shutdown()
    {
        std::cout << "Application shutting down.\n";
    }

    void Application::runFrame(Timestep timestep)
    {
        update(timestep);
        render();
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

    bool Application::shouldClose() const
    {
        return m_frameIndex >= 5;
    }
}
