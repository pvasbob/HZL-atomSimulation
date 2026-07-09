#include "hzl/renderer/VertexBuffer.h"

#include <glad/gl.h>

namespace hzl
{
    VertexBuffer::VertexBuffer(const float* data, std::size_t sizeInBytes)
    {
        glGenBuffers(1, &m_handle);
        bind();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeInBytes), data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_handle);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
