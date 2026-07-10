#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/renderer/Mesh.h"
#include "hzl/renderer/Shader.h"
#include "hzl/scene/Camera.h"
#include "hzl/scene/Transform.h"
#include "hzl/simulation/Atom.h"

#include <memory>

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
        void endFrame();

    private:
        std::unique_ptr<Mesh> m_mesh;
        std::unique_ptr<Shader> m_shader;
        Camera m_camera;
        Atom m_atom;
        Transform m_transform;
    };
}
