#pragma once

namespace hzl
{
    class Application
    {
    public:
        Application();

        void run();

    private:
        void update();
        void render();

    private:
        bool m_running = true;
        int m_frameIndex = 0;
    };
}
