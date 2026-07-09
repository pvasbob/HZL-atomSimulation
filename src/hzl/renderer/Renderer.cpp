#include "hzl/renderer/Renderer.h"

#include <glad/gl.h>

#include <array>
#include <iostream>
#include <memory>

namespace hzl
{
    namespace
    {
        constexpr const char* vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec3 a_position;
            layout (location = 1) in vec3 a_color;

            out vec3 v_color;

            void main()
            {
                v_color = a_color;
                gl_Position = vec4(a_position, 1.0);
            }
        )";

        constexpr const char* fragmentShaderSource = R"(
            #version 330 core

            in vec3 v_color;
            out vec4 frag_color;

            void main()
            {
                frag_color = vec4(v_color, 1.0);
            }
        )";
    }

    Renderer::Renderer()
        : m_shader(std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource))
    {
        constexpr std::array<float, 18> vertices = {
            // position             // color
             0.0f,  0.5f, 0.0f,     0.95f, 0.35f, 0.25f,
            -0.5f, -0.5f, 0.0f,     0.20f, 0.75f, 0.45f,
             0.5f, -0.5f, 0.0f,     0.25f, 0.45f, 0.95f,
        };

        constexpr std::array<unsigned int, 3> indices = {
            0, 1, 2,
        };

        m_vertexArray = std::make_unique<VertexArray>();
        m_vertexBuffer = std::make_unique<VertexBuffer>(
            vertices.data(),
            vertices.size() * sizeof(float));
        m_indexBuffer = std::make_unique<IndexBuffer>(
            indices.data(),
            indices.size());

        m_vertexArray->bind();
        m_vertexBuffer->bind();
        m_indexBuffer->bind();

        const BufferLayout layout = {
            {0, 3, 0},
            {1, 3, 3 * sizeof(float)},
        };
        m_vertexArray->setLayout(layout);

        m_vertexBuffer->unbind();
        m_vertexArray->unbind();

        std::cout << "Renderer initialized.\n";
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::beginFrame()
    {
        glClearColor(0.05f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();
        m_vertexArray->bind();
        m_indexBuffer->bind();
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(m_indexBuffer->count()),
            GL_UNSIGNED_INT,
            nullptr);
        m_vertexArray->unbind();
    }

    void Renderer::endFrame()
    {
    }
}
