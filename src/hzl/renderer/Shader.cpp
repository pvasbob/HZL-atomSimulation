#include "hzl/renderer/Shader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

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
    }

    Shader::Shader(const char* vertexSource, const char* fragmentSource)
    {
        const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        int success = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);

        if (success == 0)
        {
            int logLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

            std::string message(static_cast<std::size_t>(logLength), '\0');
            glGetProgramInfoLog(m_program, logLength, nullptr, message.data());
            glDeleteProgram(m_program);
            m_program = 0;

            throw std::runtime_error("Shader linking failed: " + message);
        }
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_program);
    }

    void Shader::bind() const
    {
        glUseProgram(m_program);
    }

    void Shader::setMat4(const char* name, const glm::mat4& value) const
    {
        const int location = glGetUniformLocation(m_program, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}
