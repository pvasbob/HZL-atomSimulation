#include "hzl/renderer/Renderer.h"

#include <glad/gl.h>

#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

namespace hzl
{
    namespace
    {
        unsigned int compileShader(unsigned int type, const char* source)
        {
            const unsigned int shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            int success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (success == 0)
            {
                int logLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

                std::string message(static_cast<std::size_t>(logLength), '\0');
                glGetShaderInfoLog(shader, logLength, nullptr, message.data());
                glDeleteShader(shader);

                throw std::runtime_error("Shader compilation failed: " + message);
            }

            return shader;
        }

        unsigned int createShaderProgram()
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

            const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
            const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

            const unsigned int program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            int success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);

            if (success == 0)
            {
                int logLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

                std::string message(static_cast<std::size_t>(logLength), '\0');
                glGetProgramInfoLog(program, logLength, nullptr, message.data());
                glDeleteProgram(program);

                throw std::runtime_error("Shader linking failed: " + message);
            }

            return program;
        }
    }

    Renderer::Renderer()
        : m_shaderProgram(createShaderProgram())
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
        glDeleteProgram(m_shaderProgram);
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteVertexArrays(1, &m_vertexArray);
    }

    void Renderer::beginFrame()
    {
        glClearColor(0.05f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_shaderProgram);
        glBindVertexArray(m_vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void Renderer::endFrame()
    {
    }
}
