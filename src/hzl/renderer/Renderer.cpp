#include "hzl/renderer/Renderer.h"

#include <glad/gl.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
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
            uniform mat4 u_viewProjection;

            void main()
            {
                v_color = a_color;
                gl_Position = u_viewProjection * u_model * vec4(a_position, 1.0);
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
        : m_shader(std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource)),
          m_camera(1280.0f / 720.0f, glm::radians(45.0f), 0.1f, 100.0f)
    {
        glEnable(GL_DEPTH_TEST);

        constexpr std::array<float, 48> vertices = {
            // position             // color
            -0.5f, -0.5f, -0.5f,    0.95f, 0.35f, 0.25f,
             0.5f, -0.5f, -0.5f,    0.20f, 0.75f, 0.45f,
             0.5f,  0.5f, -0.5f,    0.25f, 0.45f, 0.95f,
            -0.5f,  0.5f, -0.5f,    0.95f, 0.85f, 0.25f,
            -0.5f, -0.5f,  0.5f,    0.95f, 0.25f, 0.65f,
             0.5f, -0.5f,  0.5f,    0.25f, 0.85f, 0.85f,
             0.5f,  0.5f,  0.5f,    0.80f, 0.45f, 0.95f,
            -0.5f,  0.5f,  0.5f,    0.55f, 0.95f, 0.35f,
        };

        constexpr std::array<unsigned int, 36> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 4, 7, 7, 3, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0,
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
        m_transform.rotation.x += rotationSpeed * 0.65f * timestep.seconds();
        m_transform.rotation.y += rotationSpeed * timestep.seconds();
        m_transform.rotation.z += rotationSpeed * timestep.seconds();
    }

    void Renderer::beginFrame()
    {
        constexpr glm::vec3 clearColor(0.05f, 0.08f, 0.12f);

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader->bind();
        m_shader->setMat4("u_model", m_transform.matrix());
        m_shader->setMat4("u_viewProjection", m_camera.viewProjection());
        m_mesh->draw();
    }

    void Renderer::endFrame()
    {
    }
}
