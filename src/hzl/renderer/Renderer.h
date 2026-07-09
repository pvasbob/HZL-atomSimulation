#pragma once

#include "hzl/renderer/IndexBuffer.h"
#include "hzl/renderer/Shader.h"
#include "hzl/renderer/VertexArray.h"
#include "hzl/renderer/VertexBuffer.h"

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
        std::unique_ptr<VertexArray> m_vertexArray;
        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;
        std::unique_ptr<Shader> m_shader;
    };
}
