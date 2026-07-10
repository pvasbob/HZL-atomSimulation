#include "hzl/simulation/AtomFactory.h"

#include <algorithm>
#include <array>
#include <cstddef>

namespace hzl
{
    namespace
    {
        struct ElementRecord
        {
            const char* symbol;
            const char* name;
            int massNumber;
        };

        constexpr std::array<ElementRecord, 119> elements{{
            {"", "", 0},
            {"H", "Hydrogen", 1}, {"He", "Helium", 4},
            {"Li", "Lithium", 7}, {"Be", "Beryllium", 9}, {"B", "Boron", 11}, {"C", "Carbon", 12}, {"N", "Nitrogen", 14}, {"O", "Oxygen", 16}, {"F", "Fluorine", 19}, {"Ne", "Neon", 20},
            {"Na", "Sodium", 23}, {"Mg", "Magnesium", 24}, {"Al", "Aluminium", 27}, {"Si", "Silicon", 28}, {"P", "Phosphorus", 31}, {"S", "Sulfur", 32}, {"Cl", "Chlorine", 35}, {"Ar", "Argon", 40},
            {"K", "Potassium", 39}, {"Ca", "Calcium", 40}, {"Sc", "Scandium", 45}, {"Ti", "Titanium", 48}, {"V", "Vanadium", 51}, {"Cr", "Chromium", 52}, {"Mn", "Manganese", 55}, {"Fe", "Iron", 56}, {"Co", "Cobalt", 59}, {"Ni", "Nickel", 58}, {"Cu", "Copper", 63}, {"Zn", "Zinc", 64},
            {"Ga", "Gallium", 69}, {"Ge", "Germanium", 74}, {"As", "Arsenic", 75}, {"Se", "Selenium", 80}, {"Br", "Bromine", 79}, {"Kr", "Krypton", 84},
            {"Rb", "Rubidium", 85}, {"Sr", "Strontium", 88}, {"Y", "Yttrium", 89}, {"Zr", "Zirconium", 90}, {"Nb", "Niobium", 93}, {"Mo", "Molybdenum", 98}, {"Tc", "Technetium", 98}, {"Ru", "Ruthenium", 102}, {"Rh", "Rhodium", 103}, {"Pd", "Palladium", 106}, {"Ag", "Silver", 107}, {"Cd", "Cadmium", 114},
            {"In", "Indium", 115}, {"Sn", "Tin", 120}, {"Sb", "Antimony", 121}, {"Te", "Tellurium", 130}, {"I", "Iodine", 127}, {"Xe", "Xenon", 132},
            {"Cs", "Caesium", 133}, {"Ba", "Barium", 138}, {"La", "Lanthanum", 139}, {"Ce", "Cerium", 140}, {"Pr", "Praseodymium", 141}, {"Nd", "Neodymium", 142}, {"Pm", "Promethium", 145}, {"Sm", "Samarium", 152}, {"Eu", "Europium", 153}, {"Gd", "Gadolinium", 158}, {"Tb", "Terbium", 159}, {"Dy", "Dysprosium", 164}, {"Ho", "Holmium", 165}, {"Er", "Erbium", 166}, {"Tm", "Thulium", 169}, {"Yb", "Ytterbium", 174}, {"Lu", "Lutetium", 175},
            {"Hf", "Hafnium", 180}, {"Ta", "Tantalum", 181}, {"W", "Tungsten", 184}, {"Re", "Rhenium", 187}, {"Os", "Osmium", 192}, {"Ir", "Iridium", 193}, {"Pt", "Platinum", 195}, {"Au", "Gold", 197}, {"Hg", "Mercury", 202},
            {"Tl", "Thallium", 205}, {"Pb", "Lead", 208}, {"Bi", "Bismuth", 209}, {"Po", "Polonium", 209}, {"At", "Astatine", 210}, {"Rn", "Radon", 222},
            {"Fr", "Francium", 223}, {"Ra", "Radium", 226}, {"Ac", "Actinium", 227}, {"Th", "Thorium", 232}, {"Pa", "Protactinium", 231}, {"U", "Uranium", 238}, {"Np", "Neptunium", 237}, {"Pu", "Plutonium", 244}, {"Am", "Americium", 243}, {"Cm", "Curium", 247}, {"Bk", "Berkelium", 247}, {"Cf", "Californium", 251}, {"Es", "Einsteinium", 252}, {"Fm", "Fermium", 257}, {"Md", "Mendelevium", 258}, {"No", "Nobelium", 259}, {"Lr", "Lawrencium", 266},
            {"Rf", "Rutherfordium", 267}, {"Db", "Dubnium", 268}, {"Sg", "Seaborgium", 269}, {"Bh", "Bohrium", 270}, {"Hs", "Hassium", 277}, {"Mt", "Meitnerium", 278}, {"Ds", "Darmstadtium", 281}, {"Rg", "Roentgenium", 282}, {"Cn", "Copernicium", 285}, {"Nh", "Nihonium", 286}, {"Fl", "Flerovium", 289}, {"Mc", "Moscovium", 290}, {"Lv", "Livermorium", 293}, {"Ts", "Tennessine", 294}, {"Og", "Oganesson", 294}
        }};

        int periodForAtomicNumber(int atomicNumber)
        {
            if (atomicNumber <= 2) { return 1; }
            if (atomicNumber <= 10) { return 2; }
            if (atomicNumber <= 18) { return 3; }
            if (atomicNumber <= 36) { return 4; }
            if (atomicNumber <= 54) { return 5; }
            if (atomicNumber <= 86) { return 6; }
            return 7;
        }

        struct Subshell
        {
            int principalQuantumNumber;
            OrbitalType type;
            int capacity;
        };

        constexpr std::array<Subshell, 19> subshellFillOrder{{
            {1, OrbitalType::S, 2},
            {2, OrbitalType::S, 2},
            {2, OrbitalType::P, 6},
            {3, OrbitalType::S, 2},
            {3, OrbitalType::P, 6},
            {4, OrbitalType::S, 2},
            {3, OrbitalType::D, 10},
            {4, OrbitalType::P, 6},
            {5, OrbitalType::S, 2},
            {4, OrbitalType::D, 10},
            {5, OrbitalType::P, 6},
            {6, OrbitalType::S, 2},
            {4, OrbitalType::F, 14},
            {5, OrbitalType::D, 10},
            {6, OrbitalType::P, 6},
            {7, OrbitalType::S, 2},
            {5, OrbitalType::F, 14},
            {6, OrbitalType::D, 10},
            {7, OrbitalType::P, 6}
        }};

        glm::vec3 rainbowColorForOrbitalOrder(int orbitalOrder)
        {
            constexpr std::array<glm::vec3, 7> rainbowColors{{
                {1.00f, 0.05f, 0.03f},
                {1.00f, 0.48f, 0.03f},
                {1.00f, 0.92f, 0.08f},
                {0.10f, 1.00f, 0.22f},
                {0.18f, 0.48f, 1.00f},
                {0.32f, 0.18f, 0.90f},
                {0.82f, 0.24f, 1.00f}
            }};

            return rainbowColors[static_cast<std::size_t>(orbitalOrder % static_cast<int>(rainbowColors.size()))];
        }

        int orbitalOrderForSubshell(const Subshell& subshell)
        {
            for (int index = 0; index < static_cast<int>(subshellFillOrder.size()); ++index)
            {
                const Subshell& current = subshellFillOrder[static_cast<std::size_t>(index)];
                if (current.principalQuantumNumber == subshell.principalQuantumNumber
                    && current.type == subshell.type)
                {
                    return index;
                }
            }

            return 0;
        }

        float radiusForSubshell(const Subshell& subshell)
        {
            const float baseRadius = 0.18f + 0.105f * static_cast<float>(subshell.principalQuantumNumber);

            switch (subshell.type)
            {
                case OrbitalType::S:
                    return baseRadius;
                case OrbitalType::P:
                    return baseRadius + 0.10f;
                case OrbitalType::D:
                    return baseRadius + 0.17f;
                case OrbitalType::F:
                    return baseRadius + 0.24f;
            }

            return baseRadius;
        }

        OrbitalVisualEmphasis emphasisForSubshell(const Subshell& subshell, int highestPrincipalQuantumNumber)
        {
            if (subshell.principalQuantumNumber == highestPrincipalQuantumNumber)
            {
                return OrbitalVisualEmphasis::Active;
            }

            if (subshell.principalQuantumNumber + 1 == highestPrincipalQuantumNumber)
            {
                return OrbitalVisualEmphasis::Supporting;
            }

            return OrbitalVisualEmphasis::Core;
        }

        void addSubshellOrbital(
            Atom& atom,
            const Subshell& subshell,
            int electronCount,
            int highestPrincipalQuantumNumber)
        {
            if (electronCount <= 0)
            {
                return;
            }

            const OrbitalVisualEmphasis emphasis = emphasisForSubshell(subshell, highestPrincipalQuantumNumber);
            glm::vec3 color = rainbowColorForOrbitalOrder(orbitalOrderForSubshell(subshell));
            if (emphasis == OrbitalVisualEmphasis::Core)
            {
                color *= 0.55f;
            }

            if (subshell.type == OrbitalType::S)
            {
                atom.orbitals.push_back({
                    subshell.principalQuantumNumber,
                    subshell.type,
                    OrbitalAxis::None,
                    electronCount,
                    radiusForSubshell(subshell),
                    color,
                    emphasis});
                return;
            }

            const OrbitalAxis axes[3] = {OrbitalAxis::X, OrbitalAxis::Y, OrbitalAxis::Z};
            int remainingElectrons = electronCount;
            for (int axisIndex = 0; axisIndex < 3 && remainingElectrons > 0; ++axisIndex)
            {
                const int remainingAxes = 3 - axisIndex;
                const int axisElectrons = (remainingElectrons + remainingAxes - 1) / remainingAxes;
                atom.orbitals.push_back({
                    subshell.principalQuantumNumber,
                    subshell.type,
                    axes[axisIndex],
                    axisElectrons,
                    radiusForSubshell(subshell),
                    color,
                    emphasis});

                remainingElectrons -= axisElectrons;
            }
        }
    }

    Atom AtomFactory::createOxygen16()
    {
        Atom oxygen16;
        oxygen16.position = {0.0f, 0.0f, 0.0f};
        oxygen16.nucleusRadius = 0.045f;
        oxygen16.nucleusColor = {0.00f, 0.00f, 1.00f};
        oxygen16.atomicNumber = 8;
        oxygen16.massNumber = 16;
        oxygen16.symbol = "O";
        oxygen16.name = "Oxygen";

        oxygen16.orbitals.push_back(
            {1, OrbitalType::S, OrbitalAxis::None, 2, 0.28f, {0.55f, 0.03f, 0.02f}, OrbitalVisualEmphasis::Core});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::S, OrbitalAxis::None, 2, 0.54f, {1.00f, 0.48f, 0.03f}, OrbitalVisualEmphasis::Supporting});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::P, OrbitalAxis::X, 2, 0.78f, {1.00f, 0.92f, 0.08f}, OrbitalVisualEmphasis::Active});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::P, OrbitalAxis::Y, 1, 0.78f, {1.00f, 0.92f, 0.08f}, OrbitalVisualEmphasis::Active});
        oxygen16.orbitals.push_back(
            {2, OrbitalType::P, OrbitalAxis::Z, 1, 0.78f, {1.00f, 0.92f, 0.08f}, OrbitalVisualEmphasis::Active});

        addElectronSamples(oxygen16);

        return oxygen16;
    }

    Atom AtomFactory::createBismuth209()
    {
        return createElement(83);
    }

    Atom AtomFactory::createElement(int atomicNumber)
    {
        atomicNumber = std::clamp(atomicNumber, 1, 118);

        if (atomicNumber == 8)
        {
            return createOxygen16();
        }

        const ElementRecord& element = elements[static_cast<std::size_t>(atomicNumber)];

        Atom atom;
        atom.position = {0.0f, 0.0f, 0.0f};
        atom.nucleusRadius = 0.035f + 0.00012f * static_cast<float>(atomicNumber);
        atom.nucleusColor = {0.00f, 0.00f, 1.00f};
        atom.atomicNumber = atomicNumber;
        atom.massNumber = element.massNumber;
        atom.symbol = element.symbol;
        atom.name = element.name;

        int remainingElectrons = atomicNumber;
        int highestPrincipalQuantumNumber = 1;

        for (const Subshell& subshell : subshellFillOrder)
        {
            if (remainingElectrons <= 0)
            {
                break;
            }

            const int electronCount = std::min(remainingElectrons, subshell.capacity);
            if (electronCount > 0)
            {
                highestPrincipalQuantumNumber = std::max(highestPrincipalQuantumNumber, subshell.principalQuantumNumber);
            }

            remainingElectrons -= electronCount;
        }

        remainingElectrons = atomicNumber;
        for (const Subshell& subshell : subshellFillOrder)
        {
            if (remainingElectrons <= 0)
            {
                break;
            }

            const int electronCount = std::min(remainingElectrons, subshell.capacity);
            addSubshellOrbital(atom, subshell, electronCount, highestPrincipalQuantumNumber);
            remainingElectrons -= electronCount;
        }

        addElectronSamples(atom);

        return atom;
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
