#include "hzl/renderer/MeshFactory.h"

#include <array>

namespace hzl
{
    std::unique_ptr<Mesh> MeshFactory::createCube()
    {
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

        return std::make_unique<Mesh>(
            vertices.data(),
            vertices.size() * sizeof(float),
            indices.data(),
            indices.size(),
            layout);
    }
}
