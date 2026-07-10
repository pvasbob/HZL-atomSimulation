#pragma once

#include <glm/vec3.hpp>

#include <vector>

namespace hzl
{
    struct Electron
    {
        glm::vec3 relativePosition = {0.0f, 0.0f, 0.0f};
        float radius = 0.08f;
        glm::vec3 color = {0.20f, 0.55f, 1.0f};
        float orbitRadius = 1.0f;
        float orbitAngle = 0.0f;
        float orbitSpeed = 1.0f;
        float orbitTilt = 0.0f;
    };

    struct Atom
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        float nucleusRadius = 0.35f;
        glm::vec3 nucleusColor = {0.90f, 0.20f, 0.18f};
        int atomicNumber = 0;
        int massNumber = 0;
        std::vector<Electron> electrons;
    };
}
