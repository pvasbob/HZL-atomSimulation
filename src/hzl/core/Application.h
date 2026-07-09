#pragma once

#include "hzl/core/Timestep.h"

namespace hzl
{
    class Application
    {
    public:
        Application();

        void run();

    private:
        void update(Timestep timestep);
        void render();

    private:
        bool m_running = true;
        int m_frameIndex = 0;
    };
}
