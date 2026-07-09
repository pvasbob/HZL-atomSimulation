#pragma once

namespace hzl
{
    class Timestep
    {
    public:
        explicit Timestep(float seconds)
            : m_seconds(seconds)
        {
        }

        float seconds() const
        {
            return m_seconds;
        }

        float milliseconds() const
        {
            return m_seconds * 1000.0f;
        }

    private:
        float m_seconds = 0.0f;
    };
}
