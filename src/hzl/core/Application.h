#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/platform/Window.h"
#include "hzl/renderer/Renderer.h"
#include "hzl/simulation/AtomWorld.h"

namespace hzl
{
    class Application
    {
    public:
        Application();

        void run();

    private:
        void initialize();
        void shutdown();
        void runFrame(Timestep timestep);
        void update(Timestep timestep);
        void render();
        void initializeUi();
        void shutdownUi();
        void renderUi();
        void selectElement(int atomicNumber);

        bool shouldClose() const;

    private:
        Window m_window;
        Renderer m_renderer;
        AtomWorld m_atomWorld;
        bool m_running = true;
        bool m_uiInitialized = false;
        int m_selectedAtomicNumber = 83;
        int m_frameIndex = 0;
    };
}
