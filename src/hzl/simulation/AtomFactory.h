#pragma once

#include "hzl/simulation/Atom.h"

namespace hzl
{
    class AtomFactory
    {
    public:
        static Atom createOxygen16();
        static Atom createBismuth209();

    private:
        static void addElectronSamples(Atom& atom);
        static int samplesPerElectron(const Orbital& orbital);
        static float sampleAlpha(const Orbital& orbital);
    };
}
