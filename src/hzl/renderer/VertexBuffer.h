#pragma once

#include <cstddef>

namespace hzl
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const float* data, std::size_t sizeInBytes);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        void bind() const;
        void unbind() const;

    private:
        unsigned int m_handle = 0;
    };
}
