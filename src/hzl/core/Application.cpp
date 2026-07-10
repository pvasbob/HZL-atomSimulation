#include "hzl/core/Application.h"

#include <chrono>
#include <cmath>
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

        Atom oxygen16;
        oxygen16.position = {0.0f, 0.0f, 0.0f};
        oxygen16.nucleusRadius = 0.32f;
        oxygen16.nucleusColor = {0.90f, 0.18f, 0.14f};
        oxygen16.atomicNumber = 8;
        oxygen16.massNumber = 16;

        constexpr float electronOrbitRadius = 1.15f;
        constexpr float electronRadius = 0.04f;
        const glm::vec3 electronColor = {0.25f, 0.60f, 1.0f};
        const float electronAngles[] = {
            0.0f,
            0.7853982f,
            1.5707963f,
            2.3561945f,
            3.1415927f,
            3.9269908f,
            4.7123890f,
            5.4977871f,
        };

        for (int index = 0; index < 8; ++index)
        {
            Electron electron;
            electron.radius = electronRadius;
            electron.color = electronColor;
            electron.orbitRadius = electronOrbitRadius;
            electron.orbitAngle = electronAngles[index];
            electron.orbitSpeed = 0.8f + static_cast<float>(index % 4) * 0.25f;
            electron.orbitTilt = static_cast<float>(index) * 0.45f;
            electron.relativePosition = {
                std::cos(electron.orbitAngle) * electron.orbitRadius,
                std::sin(electron.orbitAngle + electron.orbitTilt) * electron.orbitRadius * 0.35f,
                std::sin(electron.orbitAngle) * electron.orbitRadius};
            oxygen16.electrons.push_back(electron);
        }

        m_atomWorld.addAtom(oxygen16);
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
        m_atomWorld.update(timestep);
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
        m_renderer.renderAtoms(m_atomWorld.atoms());
        m_renderer.endFrame();
    }

    bool Application::shouldClose() const
    {
        return m_window.shouldClose();
    }
}
