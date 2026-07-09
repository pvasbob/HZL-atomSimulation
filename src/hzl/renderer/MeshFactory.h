#pragma once

#include "hzl/renderer/Mesh.h"

#include <memory>

namespace hzl
{
    class MeshFactory
    {
    public:
        static std::unique_ptr<Mesh> createCube();
    };
}
