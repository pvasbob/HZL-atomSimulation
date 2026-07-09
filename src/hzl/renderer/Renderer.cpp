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

        glGenVertexArrays(1, &m_vertexArray);
        glGenBuffers(1, &m_vertexBuffer);

        glBindVertexArray(m_vertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
            vertices.data(),
            GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            6 * sizeof(float),
            reinterpret_cast<const void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        std::cout << "Renderer initialized.\n";
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteVertexArrays(1, &m_vertexArray);
    }

    void Renderer::beginFrame()
    {
        glClearColor(0.05f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();
        glBindVertexArray(m_vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void Renderer::endFrame()
    {
    }
}
