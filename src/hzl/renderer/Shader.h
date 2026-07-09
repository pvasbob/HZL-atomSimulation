#pragma once

#include <glm/mat4x4.hpp>

namespace hzl
{
    class Shader
    {
    public:
        Shader(const char* vertexSource, const char* fragmentSource);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void bind() const;
        void setMat4(const char* name, const glm::mat4& value) const;

    private:
        unsigned int m_program = 0;
    };
}
