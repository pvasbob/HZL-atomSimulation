#include "hzl/renderer/Mesh.h"

#include <glad/gl.h>

namespace hzl
{
    Mesh::Mesh(
        const float* vertices,
        std::size_t vertexDataSizeInBytes,
        const unsigned int* indices,
        std::size_t indexCount,
        const BufferLayout& layout)
        : m_vertexArray(std::make_unique<VertexArray>()),
          m_vertexBuffer(std::make_unique<VertexBuffer>(vertices, vertexDataSizeInBytes)),
          m_indexBuffer(std::make_unique<IndexBuffer>(indices, indexCount))
    {
        m_vertexArray->bind();
        m_vertexBuffer->bind();
        m_indexBuffer->bind();

        m_vertexArray->setLayout(layout);

        m_vertexBuffer->unbind();
        m_vertexArray->unbind();
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::draw() const
    {
        m_vertexArray->bind();
        m_indexBuffer->bind();
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(m_indexBuffer->count()),
            GL_UNSIGNED_INT,
            nullptr);
        m_vertexArray->unbind();
    }
}
