#pragma once

#include <glm/vec3.hpp>

#include <vector>

namespace hzl
{
    enum class OrbitalType
    {
        S,
        P
    };

    enum class OrbitalAxis
    {
        None,
        X,
        Y,
        Z
    };

    struct Orbital
    {
        int principalQuantumNumber = 1;
        OrbitalType type = OrbitalType::S;
        OrbitalAxis axis = OrbitalAxis::None;
        int electronCount = 0;
        float visualRadius = 1.0f;
        glm::vec3 color = {0.25f, 0.60f, 1.0f};
    };

    struct ElectronSample
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        float radius = 0.025f;
        glm::vec3 color = {0.85f, 0.95f, 1.0f};
        int orbitalIndex = 0;
    };

    struct Atom
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        float nucleusRadius = 0.35f;
        glm::vec3 nucleusColor = {0.90f, 0.20f, 0.18f};
        int atomicNumber = 0;
        int massNumber = 0;
        std::vector<Orbital> orbitals;
        std::vector<ElectronSample> electronSamples;
    };
}
