#include "hzl/renderer/IndexBuffer.h"

#include <glad/gl.h>

namespace hzl
{
    IndexBuffer::IndexBuffer(const unsigned int* indices, std::size_t count)
        : m_count(count)
    {
        glGenBuffers(1, &m_handle);
        bind();
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(count * sizeof(unsigned int)),
            indices,
            GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_handle);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    std::size_t IndexBuffer::count() const
    {
        return m_count;
    }
}
