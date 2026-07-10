#pragma once

#include "hzl/core/Timestep.h"
#include "hzl/simulation/Atom.h"

#include <vector>

namespace hzl
{
    class AtomWorld
    {
    public:
        void update(Timestep timestep)
        {
            (void)timestep;
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
