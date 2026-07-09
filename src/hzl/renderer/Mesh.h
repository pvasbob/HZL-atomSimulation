#pragma once

#include "hzl/renderer/BufferLayout.h"
#include "hzl/renderer/IndexBuffer.h"
#include "hzl/renderer/VertexArray.h"
#include "hzl/renderer/VertexBuffer.h"

#include <cstddef>
#include <memory>

namespace hzl
{
    class Mesh
    {
    public:
        Mesh(
            const float* vertices,
            std::size_t vertexDataSizeInBytes,
            const unsigned int* indices,
            std::size_t indexCount,
            const BufferLayout& layout);
        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void draw() const;

    private:
        std::unique_ptr<VertexArray> m_vertexArray;
        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;
    };
}
