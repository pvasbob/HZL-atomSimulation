#include "hzl/renderer/VertexArray.h"

#include <glad/gl.h>

namespace hzl
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_handle);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_handle);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_handle);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::addFloatAttribute(
        unsigned int index,
        int componentCount,
        int strideInBytes,
        const void* offset) const
    {
        glVertexAttribPointer(index, componentCount, GL_FLOAT, GL_FALSE, strideInBytes, offset);
        glEnableVertexAttribArray(index);
    }
}
