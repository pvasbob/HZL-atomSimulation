#include "hzl/simulation/AtomFactory.h"

#include <cstddef>

namespace hzl
{
    Atom AtomFactory::createOxygen16()
    {
        Atom oxygen16;
        oxygen16.position = {0.0f, 0.0f, 0.0f};
        oxygen16.nucleusRadius = 0.12f;
        oxygen16.nucleusColor = {0.02f, 0.02f, 0.025f};
        oxygen16.atomicNumber = 8;
        oxygen16.massNumber = 16;

        oxygen16.orbitals.push_back(
            {1, OrbitalType::S, OrbitalAxis::None, 2, 0.28f, {1.00f, 0.12f, 0.08f}, OrbitalVisualEmphasis::Core});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::S, OrbitalAxis::None, 2, 0.54f, {1.00f, 0.95f, 0.08f}, OrbitalVisualEmphasis::Supporting});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::P, OrbitalAxis::X, 2, 0.78f, {0.12f, 1.00f, 0.22f}, OrbitalVisualEmphasis::Active});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::P, OrbitalAxis::Y, 1, 0.78f, {0.12f, 1.00f, 0.22f}, OrbitalVisualEmphasis::Active});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::P, OrbitalAxis::Z, 1, 0.78f, {0.12f, 1.00f, 0.22f}, OrbitalVisualEmphasis::Active});

        addElectronSamples(oxygen16);

        return oxygen16;
    }

    void AtomFactory::addElectronSamples(Atom& atom)
    {
        for (int orbitalIndex = 0; orbitalIndex < static_cast<int>(atom.orbitals.size()); ++orbitalIndex)
        {
            const Orbital& orbital = atom.orbitals[static_cast<std::size_t>(orbitalIndex)];
            const int sampleCount = orbital.electronCount * samplesPerElectron(orbital);
            const float alpha = sampleAlpha(orbital);

            for (int sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex)
            {
                ElectronSample sample;
                sample.color = orbital.color;
                sample.alpha = alpha;
                sample.orbitalIndex = orbitalIndex;
                atom.electronSamples.push_back(sample);
            }
        }
    }

    int AtomFactory::samplesPerElectron(const Orbital& orbital)
    {
        switch (orbital.visualEmphasis)
        {
            case OrbitalVisualEmphasis::Core:
                return 500;
            case OrbitalVisualEmphasis::Supporting:
                return 1000;
            case OrbitalVisualEmphasis::Active:
                return 1200;
        }

        return 800;
    }

    float AtomFactory::sampleAlpha(const Orbital& orbital)
    {
        switch (orbital.visualEmphasis)
        {
            case OrbitalVisualEmphasis::Core:
                return 0.18f;
            case OrbitalVisualEmphasis::Supporting:
                return 0.28f;
            case OrbitalVisualEmphasis::Active:
                return 0.42f;
        }

        return 0.25f;
    }
}
