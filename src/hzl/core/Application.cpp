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

        oxygen16.orbitals.push_back({1, OrbitalType::S, OrbitalAxis::None, 2, 0.28f, {1.00f, 0.12f, 0.08f}});
        oxygen16.orbitals.push_back({2, OrbitalType::S, OrbitalAxis::None, 2, 0.54f, {1.00f, 0.95f, 0.08f}});
        oxygen16.orbitals.push_back({2, OrbitalType::P, OrbitalAxis::X, 2, 0.78f, {0.12f, 1.00f, 0.22f}});
        oxygen16.orbitals.push_back({2, OrbitalType::P, OrbitalAxis::Y, 1, 0.78f, {0.12f, 1.00f, 0.22f}});
        oxygen16.orbitals.push_back({2, OrbitalType::P, OrbitalAxis::Z, 1, 0.78f, {0.12f, 1.00f, 0.22f}});

        for (int orbitalIndex = 0; orbitalIndex < static_cast<int>(oxygen16.orbitals.size()); ++orbitalIndex)
        {
            const Orbital& orbital = oxygen16.orbitals[static_cast<std::size_t>(orbitalIndex)];

            const bool isSOrbital = orbital.type == OrbitalType::S;
            const int samplesPerElectron = isSOrbital ? 1500 : 650;
            const float sampleAlpha = isSOrbital
                ? (orbital.principalQuantumNumber == 1 ? 0.55f : 0.34f)
                : 0.30f;

            for (int sampleIndex = 0; sampleIndex < orbital.electronCount * samplesPerElectron; ++sampleIndex)
            {
                ElectronSample sample;
                sample.color = orbital.color;
                sample.alpha = sampleAlpha;
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
