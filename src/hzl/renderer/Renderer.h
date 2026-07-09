#pragma once

#include "hzl/renderer/Shader.h"

#include <memory>

namespace hzl
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void beginFrame();
        void endFrame();

    private:
        unsigned int m_vertexArray = 0;
        unsigned int m_vertexBuffer = 0;
        std::unique_ptr<Shader> m_shader;
    };
}
