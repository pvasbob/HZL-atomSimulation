#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/simulation/Atom.h"

#include <cmath>
#include <vector>

namespace hzl
{
    class AtomWorld
    {
    public:
        void update(Timestep timestep)
        {
            for (Atom& atom : m_atoms)
            {
                for (Electron& electron : atom.electrons)
                {
                    electron.orbitAngle += electron.orbitSpeed * timestep.seconds();

                    const float x = std::cos(electron.orbitAngle) * electron.orbitRadius;
                    const float z = std::sin(electron.orbitAngle) * electron.orbitRadius;
                    const float y = std::sin(electron.orbitAngle + electron.orbitTilt) * electron.orbitRadius * 0.35f;

                    electron.relativePosition = {x, y, z};
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
        std::vector<Atom> m_atoms;
    };
}
