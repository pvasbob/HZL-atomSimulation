#include "hzl/renderer/Renderer.h"

#include "hzl/renderer/MeshFactory.h"

#include <glad/gl.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

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

            uniform mat4 u_model;
            uniform mat4 u_viewProjection;

            void main()
            {
                gl_Position = u_viewProjection * u_model * vec4(a_position, 1.0);
            }
        )";

        constexpr const char* fragmentShaderSource = R"(
            #version 330 core

            out vec4 frag_color;

            uniform vec3 u_color;

            void main()
            {
                frag_color = vec4(u_color, 1.0);
            }
        )";
    }

    Renderer::Renderer()
        : m_shader(std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource)),
          m_camera(1280.0f / 720.0f, glm::radians(45.0f), 0.1f, 100.0f)
    {
        glEnable(GL_DEPTH_TEST);

        m_mesh = MeshFactory::createSphere(1.0f, 24, 32);

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
    }

    void Renderer::renderAtoms(const std::vector<Atom>& atoms)
    {
        m_shader->bind();
        m_shader->setMat4("u_viewProjection", m_camera.viewProjection());

        for (const Atom& atom : atoms)
        {
            m_transform.position = atom.position;
            m_transform.scale = {
                atom.nucleusRadius,
                atom.nucleusRadius,
                atom.nucleusRadius};

            m_shader->setMat4("u_model", m_transform.matrix());
            m_shader->setVec3("u_color", atom.nucleusColor);
            m_mesh->draw();

            for (const Electron& electron : atom.electrons)
            {
                m_transform.position = atom.position + electron.relativePosition;
                m_transform.scale = {
                    electron.radius,
                    electron.radius,
                    electron.radius};

                m_shader->setMat4("u_model", m_transform.matrix());
                m_shader->setVec3("u_color", electron.color);
                m_mesh->draw();
            }
        }
    }

    void Renderer::endFrame()
    {
    }
}
