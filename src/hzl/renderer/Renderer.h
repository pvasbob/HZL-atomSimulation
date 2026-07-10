#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/platform/Window.h"
#include "hzl/renderer/Mesh.h"
#include "hzl/renderer/Shader.h"
#include "hzl/scene/Camera.h"
#include "hzl/scene/Transform.h"
#include "hzl/simulation/Atom.h"

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

    private:
        void renderOrbitalMeshes(const Atom& atom);
        void renderNucleus(const Atom& atom);
        void renderSOrbital(const Atom& atom, const Orbital& orbital);
        void renderPOrbital(const Atom& atom, const Orbital& orbital);
        void renderDOrbital(const Atom& atom, const Orbital& orbital);
        void renderFOrbital(const Atom& atom, const Orbital& orbital);
        void drawScaledOrbitalLobe(const glm::vec3& position, const glm::vec3& scale, const Orbital& orbital);
        float orbitalSurfaceAlpha(const Orbital& orbital) const;
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
        unsigned int m_pointVertexArray = 0;
        unsigned int m_pointVertexBuffer = 0;
        int m_pointCount = 0;
    };
}
