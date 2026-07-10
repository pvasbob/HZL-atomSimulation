#pragma once

#include <glm/vec3.hpp>

namespace hzl
{
    struct Atom
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        float radius = 0.5f;
        glm::vec3 color = {0.35f, 0.65f, 1.0f};
    };
}
