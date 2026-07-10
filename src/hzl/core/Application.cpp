#include "hzl/core/Application.h"

#include <glm/geometric.hpp>

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

        Atom oxygen16;
        oxygen16.position = {0.0f, 0.0f, 0.0f};
        oxygen16.nucleusRadius = 0.35f;
        oxygen16.nucleusColor = {0.90f, 0.18f, 0.14f};
        oxygen16.atomicNumber = 8;
        oxygen16.massNumber = 16;

        constexpr float electronOrbitRadius = 0.95f;
        constexpr float electronRadius = 0.075f;
        const glm::vec3 electronColor = {0.25f, 0.60f, 1.0f};
        const glm::vec3 electronDirections[] = {
            {-1.0f, -1.0f, -1.0f},
            { 1.0f, -1.0f, -1.0f},
            {-1.0f,  1.0f, -1.0f},
            { 1.0f,  1.0f, -1.0f},
            {-1.0f, -1.0f,  1.0f},
            { 1.0f, -1.0f,  1.0f},
            {-1.0f,  1.0f,  1.0f},
            { 1.0f,  1.0f,  1.0f},
        };

        for (const glm::vec3& direction : electronDirections)
        {
            Electron electron;
            electron.relativePosition = glm::normalize(direction) * electronOrbitRadius;
            electron.radius = electronRadius;
            electron.color = electronColor;
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
