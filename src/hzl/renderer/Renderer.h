#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/platform/Window.h"
#include "hzl/renderer/Mesh.h"
#include "hzl/renderer/Shader.h"
#include "hzl/scene/Camera.h"
#include "hzl/scene/Transform.h"
#include "hzl/simulation/Atom.h"

#include <glm/vec2.hpp>

#include <memory>
#include <vector>

namespace hzl
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void update(Timestep timestep, Window& window, bool enableInput);
        void beginFrame();
        void renderAtoms(const std::vector<Atom>& atoms);
        void endFrame();
        void setUiHighlightedOrbitalGroup(int principalQuantumNumber, OrbitalType orbitalType);

    private:
        void renderOrbitalMeshes(const Atom& atom);
        void renderNucleus(const Atom& atom);
        void renderSOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex);
        void renderPOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex);
        void renderDOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex);
        void renderFOrbital(const Atom& atom, const Orbital& orbital, int orbitalIndex);
        void drawScaledOrbitalLobe(const glm::vec3& position, const glm::vec3& scale, const Orbital& orbital, int orbitalIndex);
        float orbitalSurfaceAlpha(const Orbital& orbital) const;
        float adjustedOrbitalAlpha(const Orbital& orbital, int orbitalIndex) const;
        glm::vec3 adjustedOrbitalColor(const Orbital& orbital, int orbitalIndex) const;
        int hoveredOrbitalIndex(const Atom& atom) const;
        bool projectToScreen(const glm::vec3& worldPosition, glm::vec2& screenPosition) const;
        void updateOrbitCamera(Window& window);
        void updateCameraPosition();
        void initializeElectronCloudRenderer();
        void uploadElectronCloud(const std::vector<Atom>& atoms);
        void drawElectronCloud();

    private:
        std::unique_ptr<Mesh> m_mesh;
        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<Shader> m_pointShader;
        Camera m_camera;
        Transform m_transform;
        bool m_isOrbiting = false;
        double m_lastCursorX = 0.0;
        double m_lastCursorY = 0.0;
        float m_cameraYaw = 0.0f;
        float m_cameraPitch = 0.22f;
        float m_cameraDistance = 3.0f;
        bool m_sceneHoverEnabled = true;
        double m_cursorX = 0.0;
        double m_cursorY = 0.0;
        int m_viewportWidth = 1280;
        int m_viewportHeight = 720;
        int m_hoveredOrbitalIndex = -1;
        int m_uiHighlightedPrincipalQuantumNumber = -1;
        OrbitalType m_uiHighlightedOrbitalType = OrbitalType::S;
        unsigned int m_pointVertexArray = 0;
        unsigned int m_pointVertexBuffer = 0;
        int m_pointCount = 0;
    };
}
