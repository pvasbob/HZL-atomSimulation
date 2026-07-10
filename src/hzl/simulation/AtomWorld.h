#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/simulation/Atom.h"

#include <cmath>
#include <random>
#include <vector>

namespace hzl
{
    class AtomWorld
    {
    public:
        void update(Timestep timestep)
        {
            m_sampleTimer += timestep.seconds();
            if (m_sampleTimer < m_sampleIntervalSeconds)
            {
                return;
            }

            m_sampleTimer = 0.0f;

            for (Atom& atom : m_atoms)
            {
                for (ElectronSample& sample : atom.electronSamples)
                {
                    if (sample.orbitalIndex < 0
                        || sample.orbitalIndex >= static_cast<int>(atom.orbitals.size()))
                    {
                        continue;
                    }

                    sample.position = sampleOrbital(atom.orbitals[static_cast<std::size_t>(sample.orbitalIndex)]);
                }
            }
        }

        void addAtom(const Atom& atom)
        {
            m_atoms.push_back(atom);
        }

        const std::vector<Atom>& atoms() const
        {
            return m_atoms;
        }

    private:
        glm::vec3 sampleOrbital(const Orbital& orbital)
        {
            if (orbital.type == OrbitalType::P)
            {
                return samplePOrbital(orbital);
            }

            return sampleSOrbital(orbital.visualRadius);
        }

        glm::vec3 sampleSOrbital(float radius)
        {
            const float theta = random01() * 2.0f * pi;
            const float z = randomRange(-1.0f, 1.0f);
            const float radialScale = radius * std::cbrt(random01());
            const float xy = std::sqrt(1.0f - z * z);

            return {
                radialScale * xy * std::cos(theta),
                radialScale * z,
                radialScale * xy * std::sin(theta)};
        }

        glm::vec3 samplePOrbital(const Orbital& orbital)
        {
            const glm::vec3 axis = orbitalAxisDirection(orbital.axis);
            const float side = random01() < 0.5f ? -1.0f : 1.0f;
            const float alongAxis = side * randomRange(0.22f, 0.48f) * orbital.visualRadius;
            const float spread = orbital.visualRadius * 0.13f;

            glm::vec3 point = axis * alongAxis;
            point += perpendicularJitter(orbital.axis, spread);

            return point;
        }

        glm::vec3 orbitalAxisDirection(OrbitalAxis axis) const
        {
            switch (axis)
            {
                case OrbitalAxis::X:
                    return {1.0f, 0.0f, 0.0f};
                case OrbitalAxis::Y:
                    return {0.0f, 1.0f, 0.0f};
                case OrbitalAxis::Z:
                    return {0.0f, 0.0f, 1.0f};
                case OrbitalAxis::None:
                    return {0.0f, 0.0f, 0.0f};
            }

            return {0.0f, 0.0f, 0.0f};
        }

        glm::vec3 perpendicularJitter(OrbitalAxis axis, float spread)
        {
            const float a = randomRange(-spread, spread);
            const float b = randomRange(-spread, spread);

            switch (axis)
            {
                case OrbitalAxis::X:
                    return {0.0f, a, b};
                case OrbitalAxis::Y:
                    return {a, 0.0f, b};
                case OrbitalAxis::Z:
                    return {a, b, 0.0f};
                case OrbitalAxis::None:
                    return {a, b, randomRange(-spread, spread)};
            }

            return {0.0f, 0.0f, 0.0f};
        }

        float random01()
        {
            return m_distribution(m_randomEngine);
        }

        float randomRange(float min, float max)
        {
            return min + (max - min) * random01();
        }

    private:
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float m_sampleIntervalSeconds = 0.035f;
        float m_sampleTimer = 0.0f;
        std::mt19937 m_randomEngine{1337};
        std::uniform_real_distribution<float> m_distribution{0.0f, 1.0f};
        std::vector<Atom> m_atoms;
    };
}
