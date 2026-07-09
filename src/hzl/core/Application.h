#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/platform/Window.h"

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
        bool m_running = true;
        int m_frameIndex = 0;
    };
}
