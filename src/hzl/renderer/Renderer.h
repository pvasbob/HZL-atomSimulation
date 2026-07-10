#pragma once

#include "hzl/core/Timestep.h"
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

        void update(Timestep timestep);
        void beginFrame();
        void renderAtoms(const std::vector<Atom>& atoms);
        void endFrame();

    private:
        void initializeElectronCloudRenderer();
        void uploadElectronCloud(const std::vector<Atom>& atoms);
        void drawElectronCloud();

    private:
        std::unique_ptr<Mesh> m_mesh;
        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<Shader> m_pointShader;
        Camera m_camera;
        Transform m_transform;
        unsigned int m_pointVertexArray = 0;
        unsigned int m_pointVertexBuffer = 0;
        int m_pointCount = 0;
    };
}
