#pragma once

#include "hzl/renderer/Mesh.h"
#include "hzl/renderer/Shader.h"
#include "hzl/scene/Transform.h"

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

        void beginFrame();
        void endFrame();

    private:
        std::unique_ptr<Mesh> m_mesh;
        std::unique_ptr<Shader> m_shader;
        Transform m_transform;
    };
}
