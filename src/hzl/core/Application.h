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

        bool shouldClose() const;

    private:
        Window m_window;
        Renderer m_renderer;
        AtomWorld m_atomWorld;
        bool m_running = true;
        int m_frameIndex = 0;
    };
}
