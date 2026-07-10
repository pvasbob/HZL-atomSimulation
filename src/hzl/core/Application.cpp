#include "hzl/core/Application.h"

#include <chrono>
#include <cstddef>
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
        oxygen16.nucleusRadius = 0.12f;
        oxygen16.nucleusColor = {0.02f, 0.02f, 0.025f};
        oxygen16.atomicNumber = 8;
        oxygen16.massNumber = 16;

        oxygen16.orbitals.push_back({1, OrbitalType::S, OrbitalAxis::None, 2, 0.35f, {0.25f, 0.55f, 1.0f}});
        oxygen16.orbitals.push_back({2, OrbitalType::S, OrbitalAxis::None, 2, 0.62f, {0.20f, 0.75f, 1.0f}});
        oxygen16.orbitals.push_back({2, OrbitalType::P, OrbitalAxis::X, 2, 0.82f, {0.65f, 0.45f, 1.0f}});
        oxygen16.orbitals.push_back({2, OrbitalType::P, OrbitalAxis::Y, 1, 0.82f, {0.65f, 0.45f, 1.0f}});
        oxygen16.orbitals.push_back({2, OrbitalType::P, OrbitalAxis::Z, 1, 0.82f, {0.65f, 0.45f, 1.0f}});

        for (int orbitalIndex = 0; orbitalIndex < static_cast<int>(oxygen16.orbitals.size()); ++orbitalIndex)
        {
            const Orbital& orbital = oxygen16.orbitals[static_cast<std::size_t>(orbitalIndex)];

            for (int electron = 0; electron < orbital.electronCount; ++electron)
            {
                ElectronSample sample;
                sample.radius = 0.025f;
                sample.color = {0.86f, 0.95f, 1.0f};
                sample.orbitalIndex = orbitalIndex;
                oxygen16.electronSamples.push_back(sample);
            }
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
