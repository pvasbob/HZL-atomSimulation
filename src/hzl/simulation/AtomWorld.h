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

        void addAtom(Atom atom)
        {
            initializeElectronSamples(atom);
            m_atoms.push_back(atom);
        }

        void setAtom(Atom atom)
        {
            initializeElectronSamples(atom);
            m_atoms.clear();
            m_atoms.push_back(atom);
        }

        const std::vector<Atom>& atoms() const
        {
            return m_atoms;
        }

    private:
        void initializeElectronSamples(Atom& atom)
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

        glm::vec3 sampleOrbital(const Orbital& orbital)
        {
            if (orbital.type == OrbitalType::P)
            {
                return samplePOrbital(orbital);
            }

            return sampleSOrbital(orbital);
        }

        glm::vec3 sampleSOrbital(const Orbital& orbital)
        {
            const float theta = random01() * 2.0f * pi;
            const float z = randomRange(-1.0f, 1.0f);
            const float radialScale = orbital.visualRadius * randomSRadius(orbital.principalQuantumNumber);
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
            const float lobeCenter = randomRange(0.52f, 0.88f) * orbital.visualRadius;
            const float alongAxis = side * lobeCenter;
            const float distanceFromLobeCenter = std::abs(lobeCenter - 0.70f * orbital.visualRadius);
            const float lobeThickness = 0.24f * orbital.visualRadius - 0.45f * distanceFromLobeCenter;
            const float spread = std::max(0.04f * orbital.visualRadius, lobeThickness);

            glm::vec3 point = axis * alongAxis;
            point += perpendicularDiskJitter(orbital.axis, spread);

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

        glm::vec3 perpendicularDiskJitter(OrbitalAxis axis, float radius)
        {
            const float angle = random01() * 2.0f * pi;
            const float diskRadius = radius * std::sqrt(random01());
            const float a = diskRadius * std::cos(angle);
            const float b = diskRadius * std::sin(angle);

            switch (axis)
            {
                case OrbitalAxis::X:
                    return {0.0f, a, b};
                case OrbitalAxis::Y:
                    return {a, 0.0f, b};
                case OrbitalAxis::Z:
                    return {a, b, 0.0f};
                case OrbitalAxis::None:
                    return {a, b, 0.0f};
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

        float randomSRadius(int principalQuantumNumber)
        {
            if (principalQuantumNumber == 1)
            {
                return std::pow(random01(), 1.35f);
            }

            const float innerRegion = 0.25f;
            const float outerRegion = 0.75f * std::pow(random01(), 0.65f);
            return innerRegion + outerRegion;
        }

    private:
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float m_sampleIntervalSeconds = 0.025f;
        float m_sampleTimer = 0.0f;
        std::mt19937 m_randomEngine{1337};
        std::uniform_real_distribution<float> m_distribution{0.0f, 1.0f};
        std::vector<Atom> m_atoms;
    };
}
