#include "hzl/core/Application.h"

#include <chrono>
#include <iostream>

namespace hzl
{
    Application::Application()
        : m_window(WindowProperties{})
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

            m_window.onUpdate();

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
        m_renderer.update(timestep);

        if (m_frameIndex == 0)
        {
            std::cout << "First frame dt = "
                      << timestep.milliseconds() << " ms\n";
        }
    }

    void Application::render()
    {
        m_renderer.beginFrame();
        m_renderer.endFrame();
    }

    bool Application::shouldClose() const
    {
        return m_window.shouldClose();
    }
}
