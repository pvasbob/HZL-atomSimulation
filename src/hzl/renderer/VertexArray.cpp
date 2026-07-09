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

    void VertexArray::setLayout(const BufferLayout& layout) const
    {
        for (const BufferElement& element : layout.elements())
        {
            glVertexAttribPointer(
                element.index,
                element.componentCount,
                GL_FLOAT,
                GL_FALSE,
                static_cast<GLsizei>(layout.stride()),
                reinterpret_cast<const void*>(element.offset));
            glEnableVertexAttribArray(element.index);
        }
    }
}
