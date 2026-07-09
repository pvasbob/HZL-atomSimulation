#pragma once

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

    private:
        unsigned int m_program = 0;
    };
}
