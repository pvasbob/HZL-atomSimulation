#include "hzl/renderer/Renderer.h"

#include <glad/gl.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/vec3.hpp>

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

            uniform mat4 u_model;

            void main()
            {
                v_color = a_color;
                gl_Position = u_model * vec4(a_position, 1.0);
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

        const BufferLayout layout = {
            {0, 3, 0},
            {1, 3, 3 * sizeof(float)},
        };

        m_mesh = std::make_unique<Mesh>(
            vertices.data(),
            vertices.size() * sizeof(float),
            indices.data(),
            indices.size(),
            layout);

        std::cout << "Renderer initialized.\n";
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::update(Timestep timestep)
    {
        constexpr float rotationSpeed = glm::pi<float>() * 0.5f;
        m_transform.rotation.z += rotationSpeed * timestep.seconds();
    }

    void Renderer::beginFrame()
    {
        constexpr glm::vec3 clearColor(0.05f, 0.08f, 0.12f);

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();
        m_shader->setMat4("u_model", m_transform.matrix());
        m_mesh->draw();
    }

    void Renderer::endFrame()
    {
    }
}
