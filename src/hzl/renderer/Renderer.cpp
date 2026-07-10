#include "hzl/renderer/Renderer.h"

#include "hzl/renderer/MeshFactory.h"

#include <glad/gl.h>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

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
            uniform float u_alpha;

            void main()
            {
                frag_color = vec4(u_color, u_alpha);
            }
        )";

        constexpr const char* pointVertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec3 a_position;
            layout (location = 1) in vec3 a_color;
            layout (location = 2) in float a_alpha;

            out vec4 v_color;

            uniform mat4 u_model;
            uniform mat4 u_viewProjection;
            uniform float u_pointSize;

            void main()
            {
                v_color = vec4(a_color, a_alpha);
                gl_Position = u_viewProjection * u_model * vec4(a_position, 1.0);
                gl_PointSize = u_pointSize;
            }
        )";

        constexpr const char* pointFragmentShaderSource = R"(
            #version 330 core

            in vec4 v_color;
            out vec4 frag_color;

            void main()
            {
                vec2 centered = gl_PointCoord * 2.0 - 1.0;
                float distanceFromCenter = dot(centered, centered);

                if (distanceFromCenter > 1.0)
                {
                    discard;
                }

                float softAlpha = v_color.a * (1.0 - smoothstep(0.15, 1.0, distanceFromCenter));
                frag_color = vec4(v_color.rgb, softAlpha);
            }
        )";

    }

    Renderer::Renderer()
        : m_shader(std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource)),
          m_pointShader(std::make_unique<Shader>(pointVertexShaderSource, pointFragmentShaderSource)),
          m_camera(1280.0f / 720.0f, glm::radians(45.0f), 0.1f, 100.0f)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_mesh = MeshFactory::createSphere(1.0f, 24, 32);
        initializeElectronCloudRenderer();
        updateCameraPosition();

        std::cout << "Renderer initialized.\n";
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_pointVertexBuffer);
        glDeleteVertexArrays(1, &m_pointVertexArray);
    }

    void Renderer::update(Timestep timestep, Window& window, bool enableInput)
    {
        (void)timestep;
        m_sceneHoverEnabled = enableInput;
        m_viewportWidth = window.properties().width;
        m_viewportHeight = window.properties().height;

        const auto [cursorX, cursorY] = window.cursorPosition();
        m_cursorX = cursorX;
        m_cursorY = cursorY;

        if (enableInput)
        {
            updateOrbitCamera(window);
        }
        else
        {
            m_isOrbiting = false;
            (void)window.consumeScrollDeltaY();
        }
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
            renderOrbitalMeshes(atom);
            renderNucleus(atom);
        }
    }

    void Renderer::renderOrbitalMeshes(const Atom& atom)
    {
        m_hoveredOrbitalIndex = -1;

        glDepthMask(GL_FALSE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for (int orbitalIndex = 0; orbitalIndex < static_cast<int>(atom.orbitals.size()); ++orbitalIndex)
        {
            const Orbital& orbital = atom.orbitals[static_cast<std::size_t>(orbitalIndex)];

            switch (orbital.type)
            {
                case OrbitalType::S:
                    renderSOrbital(atom, orbital, orbitalIndex);
                    break;
                case OrbitalType::P:
                    renderPOrbital(atom, orbital, orbitalIndex);
                    break;
                case OrbitalType::D:
                    renderDOrbital(atom, orbital, orbitalIndex);
                    break;
                case OrbitalType::F:
                    renderFOrbital(atom, orbital, orbitalIndex);
                    break;
            }
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthMask(GL_TRUE);
    }

    void Renderer::renderNucleus(const Atom& atom)
    {
        m_transform.position = atom.position;
        m_transform.rotation = {0.0f, 0.0f, 0.0f};
        m_transform.scale = {
            atom.nucleusRadius,
            atom.nucleusRadius,
            atom.nucleusRadius};

        m_shader->setMat4("u_model", m_transform.matrix());
        m_shader->setVec3("u_color", atom.nucleusColor);
        m_shader->setFloat("u_alpha", 1.0f);
        m_mesh->draw();
    }

    void Renderer::renderSOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex)
    {
        m_transform.position = atom.position;
        m_transform.rotation = {0.0f, 0.0f, 0.0f};
        m_transform.scale = {
            orbital.visualRadius,
            orbital.visualRadius,
            orbital.visualRadius};

        m_shader->setMat4("u_model", m_transform.matrix());
        m_shader->setVec3("u_color", adjustedOrbitalColor(orbital, orbitalIndex));
        m_shader->setFloat("u_alpha", adjustedOrbitalAlpha(orbital, orbitalIndex));
        m_mesh->draw();
    }

    void Renderer::renderPOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex)
    {
        const float lobeOffset = orbital.visualRadius * 0.48f;
        const float lobeLength = orbital.visualRadius * 0.34f;
        const float lobeThickness = orbital.visualRadius * 0.20f;

        for (const float side : {-1.0f, 1.0f})
        {
            glm::vec3 offset{0.0f, 0.0f, 0.0f};
            glm::vec3 scale{lobeThickness, lobeThickness, lobeThickness};

            switch (orbital.axis)
            {
                case OrbitalAxis::X:
                    offset.x = side * lobeOffset;
                    scale.x = lobeLength;
                    break;
                case OrbitalAxis::Y:
                    offset.y = side * lobeOffset;
                    scale.y = lobeLength;
                    break;
                case OrbitalAxis::Z:
                    offset.z = side * lobeOffset;
                    scale.z = lobeLength;
                    break;
                case OrbitalAxis::None:
                    break;
            }

            m_transform.position = atom.position + offset;
            drawScaledOrbitalLobe(m_transform.position, scale, orbital, orbitalIndex);
        }
    }

    void Renderer::renderDOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex)
    {
        const float offset = orbital.visualRadius * 0.38f;
        const float length = orbital.visualRadius * 0.24f;
        const float thickness = orbital.visualRadius * 0.15f;

        const glm::vec3 directions[4] = {
            {1.0f, 1.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f},
            {1.0f, -1.0f, 0.0f},
            {-1.0f, -1.0f, 0.0f}};

        for (glm::vec3 direction : directions)
        {
            direction = glm::normalize(direction);
            glm::vec3 lobePosition = atom.position;
            glm::vec3 scale{thickness, thickness, thickness};

            switch (orbital.axis)
            {
                case OrbitalAxis::X:
                    lobePosition += glm::vec3{0.0f, direction.x * offset, direction.y * offset};
                    scale.y = length;
                    scale.z = length;
                    break;
                case OrbitalAxis::Y:
                    lobePosition += glm::vec3{direction.x * offset, 0.0f, direction.y * offset};
                    scale.x = length;
                    scale.z = length;
                    break;
                case OrbitalAxis::Z:
                case OrbitalAxis::None:
                    lobePosition += glm::vec3{direction.x * offset, direction.y * offset, 0.0f};
                    scale.x = length;
                    scale.y = length;
                    break;
            }

            drawScaledOrbitalLobe(lobePosition, scale, orbital, orbitalIndex);
        }
    }

    void Renderer::renderFOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex)
    {
        const float offset = orbital.visualRadius * 0.42f;
        const float length = orbital.visualRadius * 0.20f;
        const float thickness = orbital.visualRadius * 0.12f;

        const glm::vec3 directions[8] = {
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f}};

        for (glm::vec3 direction : directions)
        {
            direction = glm::normalize(direction);
            const glm::vec3 lobePosition = atom.position + direction * offset;
            const glm::vec3 scale{
                direction.x == 0.0f ? thickness : length,
                direction.y == 0.0f ? thickness : length,
                direction.z == 0.0f ? thickness : length};

            drawScaledOrbitalLobe(lobePosition, scale, orbital, orbitalIndex);
        }
    }

    void Renderer::drawScaledOrbitalLobe(const glm::vec3& position, const glm::vec3& scale, const Orbital& orbital, int orbitalIndex)
    {
        m_transform.position = position;
        m_transform.rotation = {0.0f, 0.0f, 0.0f};
        m_transform.scale = scale;

        m_shader->setMat4("u_model", m_transform.matrix());
        m_shader->setVec3("u_color", adjustedOrbitalColor(orbital, orbitalIndex));
        m_shader->setFloat("u_alpha", adjustedOrbitalAlpha(orbital, orbitalIndex));
        m_mesh->draw();
    }

    float Renderer::orbitalSurfaceAlpha(const Orbital& orbital) const
    {
        switch (orbital.visualEmphasis)
        {
            case OrbitalVisualEmphasis::Core:
                return 0.22f;
            case OrbitalVisualEmphasis::Supporting:
                return 0.32f;
            case OrbitalVisualEmphasis::Active:
                return 0.48f;
        }

        return 0.18f;
    }

    float Renderer::adjustedOrbitalAlpha(const Orbital& orbital, int orbitalIndex) const
    {
        const float baseAlpha = orbitalSurfaceAlpha(orbital);
        if (m_uiHighlightedPrincipalQuantumNumber < 0)
        {
            return baseAlpha;
        }

        if (orbital.principalQuantumNumber == m_uiHighlightedPrincipalQuantumNumber
            && orbital.type == m_uiHighlightedOrbitalType)
        {
            return std::min(baseAlpha * 2.0f, 0.92f);
        }

        return baseAlpha * 0.18f;
    }

    glm::vec3 Renderer::adjustedOrbitalColor(const Orbital& orbital, int orbitalIndex) const
    {
        if (m_uiHighlightedPrincipalQuantumNumber < 0)
        {
            return orbital.color;
        }

        if (orbital.principalQuantumNumber == m_uiHighlightedPrincipalQuantumNumber
            && orbital.type == m_uiHighlightedOrbitalType)
        {
            return {
                std::min(orbital.color.r * 1.55f, 1.0f),
                std::min(orbital.color.g * 1.55f, 1.0f),
                std::min(orbital.color.b * 1.55f, 1.0f)};
        }

        return orbital.color * 0.35f;
    }

    int Renderer::hoveredOrbitalIndex(const Atom& atom) const
    {
        if (!m_sceneHoverEnabled || m_viewportWidth <= 0 || m_viewportHeight <= 0)
        {
            return -1;
        }

        glm::vec2 centerScreen;
        if (!projectToScreen(atom.position, centerScreen))
        {
            return -1;
        }

        const glm::vec2 mousePosition{
            static_cast<float>(m_cursorX),
            static_cast<float>(m_cursorY)};

        int bestOrbitalIndex = -1;
        float bestDistanceToShell = 100000.0f;

        for (int orbitalIndex = 0; orbitalIndex < static_cast<int>(atom.orbitals.size()); ++orbitalIndex)
        {
            const Orbital& orbital = atom.orbitals[static_cast<std::size_t>(orbitalIndex)];

            glm::vec2 radiusScreens[3];
            const bool canProjectX = projectToScreen(atom.position + glm::vec3{orbital.visualRadius, 0.0f, 0.0f}, radiusScreens[0]);
            const bool canProjectY = projectToScreen(atom.position + glm::vec3{0.0f, orbital.visualRadius, 0.0f}, radiusScreens[1]);
            const bool canProjectZ = projectToScreen(atom.position + glm::vec3{0.0f, 0.0f, orbital.visualRadius}, radiusScreens[2]);

            float screenRadius = 0.0f;
            if (canProjectX)
            {
                screenRadius = std::max(screenRadius, glm::length(radiusScreens[0] - centerScreen));
            }
            if (canProjectY)
            {
                screenRadius = std::max(screenRadius, glm::length(radiusScreens[1] - centerScreen));
            }
            if (canProjectZ)
            {
                screenRadius = std::max(screenRadius, glm::length(radiusScreens[2] - centerScreen));
            }

            if (screenRadius <= 1.0f)
            {
                continue;
            }

            const float mouseDistanceFromCenter = glm::length(mousePosition - centerScreen);
            const float distanceToShell = std::abs(mouseDistanceFromCenter - screenRadius);
            constexpr float hoverThresholdPixels = 22.0f;

            if (distanceToShell < hoverThresholdPixels && distanceToShell < bestDistanceToShell)
            {
                bestDistanceToShell = distanceToShell;
                bestOrbitalIndex = orbitalIndex;
            }
        }

        return bestOrbitalIndex;
    }

    bool Renderer::projectToScreen(const glm::vec3& worldPosition, glm::vec2& screenPosition) const
    {
        const glm::vec4 clipPosition = m_camera.viewProjection() * glm::vec4{worldPosition, 1.0f};
        if (clipPosition.w <= 0.0f)
        {
            return false;
        }

        const glm::vec3 ndc{
            clipPosition.x / clipPosition.w,
            clipPosition.y / clipPosition.w,
            clipPosition.z / clipPosition.w};

        if (ndc.z < -1.0f || ndc.z > 1.0f)
        {
            return false;
        }

        screenPosition = {
            (ndc.x * 0.5f + 0.5f) * static_cast<float>(m_viewportWidth),
            (0.5f - ndc.y * 0.5f) * static_cast<float>(m_viewportHeight)};

        return true;
    }

    void Renderer::updateOrbitCamera(Window& window)
    {
        const auto [cursorX, cursorY] = window.cursorPosition();

        if (window.isLeftMouseButtonPressed())
        {
            if (m_isOrbiting)
            {
                constexpr float orbitSensitivity = 0.006f;
                const double deltaX = cursorX - m_lastCursorX;
                const double deltaY = cursorY - m_lastCursorY;

                m_cameraYaw -= static_cast<float>(deltaX) * orbitSensitivity;
                m_cameraPitch -= static_cast<float>(deltaY) * orbitSensitivity;
                m_cameraPitch = std::clamp(m_cameraPitch, -1.35f, 1.35f);
            }

            m_isOrbiting = true;
        }
        else
        {
            m_isOrbiting = false;
        }

        m_lastCursorX = cursorX;
        m_lastCursorY = cursorY;

        const double scrollDeltaY = window.consumeScrollDeltaY();
        if (scrollDeltaY != 0.0)
        {
            m_cameraDistance -= static_cast<float>(scrollDeltaY) * 0.18f;
            m_cameraDistance = std::clamp(m_cameraDistance, 1.4f, 8.0f);
        }

        updateCameraPosition();
    }

    void Renderer::updateCameraPosition()
    {
        const float horizontalDistance = m_cameraDistance * std::cos(m_cameraPitch);

        m_camera.setPosition({
            horizontalDistance * std::sin(m_cameraYaw),
            m_cameraDistance * std::sin(m_cameraPitch),
            horizontalDistance * std::cos(m_cameraYaw)});
    }

    void Renderer::endFrame()
    {
    }

    void Renderer::setUiHighlightedOrbitalGroup(int principalQuantumNumber, OrbitalType orbitalType)
    {
        m_uiHighlightedPrincipalQuantumNumber = principalQuantumNumber;
        m_uiHighlightedOrbitalType = orbitalType;
        m_hoveredOrbitalIndex = principalQuantumNumber >= 0 ? 0 : -1;
    }

    void Renderer::initializeElectronCloudRenderer()
    {
        glGenVertexArrays(1, &m_pointVertexArray);
        glGenBuffers(1, &m_pointVertexBuffer);

        glBindVertexArray(m_pointVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_pointVertexBuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            7 * sizeof(float),
            reinterpret_cast<const void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            2,
            1,
            GL_FLOAT,
            GL_FALSE,
            7 * sizeof(float),
            reinterpret_cast<const void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer::uploadElectronCloud(const std::vector<Atom>& atoms)
    {
        std::vector<float> points;

        for (const Atom& atom : atoms)
        {
            for (const ElectronSample& sample : atom.electronSamples)
            {
                const glm::vec3 position = atom.position + sample.position;

                points.push_back(position.x);
                points.push_back(position.y);
                points.push_back(position.z);
                points.push_back(sample.color.r);
                points.push_back(sample.color.g);
                points.push_back(sample.color.b);
                points.push_back(sample.alpha);
            }
        }

        m_pointCount = static_cast<int>(points.size() / 7);

        glBindBuffer(GL_ARRAY_BUFFER, m_pointVertexBuffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(points.size() * sizeof(float)),
            points.data(),
            GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::drawElectronCloud()
    {
        if (m_pointCount == 0)
        {
            return;
        }

        m_pointShader->bind();
        m_pointShader->setMat4("u_model", glm::mat4(1.0f));
        m_pointShader->setMat4("u_viewProjection", m_camera.viewProjection());
        m_pointShader->setFloat("u_pointSize", 1.7f);

        glDepthMask(GL_FALSE);
        glBindVertexArray(m_pointVertexArray);
        glDrawArrays(GL_POINTS, 0, m_pointCount);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }
}
