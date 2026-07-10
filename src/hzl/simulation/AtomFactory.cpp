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

    Atom AtomFactory::createBismuth209()
    {
        Atom bismuth209;
        bismuth209.position = {0.0f, 0.0f, 0.0f};
        bismuth209.nucleusRadius = 0.14f;
        bismuth209.nucleusColor = {0.015f, 0.015f, 0.020f};
        bismuth209.atomicNumber = 83;
        bismuth209.massNumber = 209;

        // Inner closed shells are drawn as subdued context instead of 78 separate core electrons.
        bismuth209.orbitals.push_back(
            {1, OrbitalType::S, OrbitalAxis::None, 78, 0.30f, {0.45f, 0.50f, 0.58f}, OrbitalVisualEmphasis::Core});

        // Approximate filled 5d10 as a soft spherical supporting shell until D-orbital shapes exist.
        bismuth209.orbitals.push_back(
            {5, OrbitalType::S, OrbitalAxis::None, 10, 0.54f, {0.36f, 0.56f, 1.00f}, OrbitalVisualEmphasis::Supporting});

        bismuth209.orbitals.push_back(
            {6, OrbitalType::S, OrbitalAxis::None, 2, 0.66f, {1.00f, 0.92f, 0.08f}, OrbitalVisualEmphasis::Supporting});

        bismuth209.orbitals.push_back(
            {6, OrbitalType::P, OrbitalAxis::X, 1, 0.88f, {0.10f, 1.00f, 0.22f}, OrbitalVisualEmphasis::Active});
        bismuth209.orbitals.push_back(
            {6, OrbitalType::P, OrbitalAxis::Y, 1, 0.88f, {0.10f, 1.00f, 0.22f}, OrbitalVisualEmphasis::Active});
        bismuth209.orbitals.push_back(
            {6, OrbitalType::P, OrbitalAxis::Z, 1, 0.88f, {0.10f, 1.00f, 0.22f}, OrbitalVisualEmphasis::Active});

        addElectronSamples(bismuth209);

        return bismuth209;
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
