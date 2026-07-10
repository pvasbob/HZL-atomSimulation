#include "hzl/core/Application.h"

#include "hzl/simulation/AtomFactory.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace hzl
{
    namespace
    {
        struct ElementButton
        {
            int atomicNumber;
            const char* symbol;
            const char* name;
            int group;
            int period;
        };

        constexpr std::array<ElementButton, 118> periodicTable{{
            {1, "H", "Hydrogen", 1, 1}, {2, "He", "Helium", 18, 1},
            {3, "Li", "Lithium", 1, 2}, {4, "Be", "Beryllium", 2, 2}, {5, "B", "Boron", 13, 2}, {6, "C", "Carbon", 14, 2}, {7, "N", "Nitrogen", 15, 2}, {8, "O", "Oxygen", 16, 2}, {9, "F", "Fluorine", 17, 2}, {10, "Ne", "Neon", 18, 2},
            {11, "Na", "Sodium", 1, 3}, {12, "Mg", "Magnesium", 2, 3}, {13, "Al", "Aluminium", 13, 3}, {14, "Si", "Silicon", 14, 3}, {15, "P", "Phosphorus", 15, 3}, {16, "S", "Sulfur", 16, 3}, {17, "Cl", "Chlorine", 17, 3}, {18, "Ar", "Argon", 18, 3},
            {19, "K", "Potassium", 1, 4}, {20, "Ca", "Calcium", 2, 4}, {21, "Sc", "Scandium", 3, 4}, {22, "Ti", "Titanium", 4, 4}, {23, "V", "Vanadium", 5, 4}, {24, "Cr", "Chromium", 6, 4}, {25, "Mn", "Manganese", 7, 4}, {26, "Fe", "Iron", 8, 4}, {27, "Co", "Cobalt", 9, 4}, {28, "Ni", "Nickel", 10, 4}, {29, "Cu", "Copper", 11, 4}, {30, "Zn", "Zinc", 12, 4}, {31, "Ga", "Gallium", 13, 4}, {32, "Ge", "Germanium", 14, 4}, {33, "As", "Arsenic", 15, 4}, {34, "Se", "Selenium", 16, 4}, {35, "Br", "Bromine", 17, 4}, {36, "Kr", "Krypton", 18, 4},
            {37, "Rb", "Rubidium", 1, 5}, {38, "Sr", "Strontium", 2, 5}, {39, "Y", "Yttrium", 3, 5}, {40, "Zr", "Zirconium", 4, 5}, {41, "Nb", "Niobium", 5, 5}, {42, "Mo", "Molybdenum", 6, 5}, {43, "Tc", "Technetium", 7, 5}, {44, "Ru", "Ruthenium", 8, 5}, {45, "Rh", "Rhodium", 9, 5}, {46, "Pd", "Palladium", 10, 5}, {47, "Ag", "Silver", 11, 5}, {48, "Cd", "Cadmium", 12, 5}, {49, "In", "Indium", 13, 5}, {50, "Sn", "Tin", 14, 5}, {51, "Sb", "Antimony", 15, 5}, {52, "Te", "Tellurium", 16, 5}, {53, "I", "Iodine", 17, 5}, {54, "Xe", "Xenon", 18, 5},
            {55, "Cs", "Caesium", 1, 6}, {56, "Ba", "Barium", 2, 6}, {57, "La", "Lanthanum", 3, 6}, {72, "Hf", "Hafnium", 4, 6}, {73, "Ta", "Tantalum", 5, 6}, {74, "W", "Tungsten", 6, 6}, {75, "Re", "Rhenium", 7, 6}, {76, "Os", "Osmium", 8, 6}, {77, "Ir", "Iridium", 9, 6}, {78, "Pt", "Platinum", 10, 6}, {79, "Au", "Gold", 11, 6}, {80, "Hg", "Mercury", 12, 6}, {81, "Tl", "Thallium", 13, 6}, {82, "Pb", "Lead", 14, 6}, {83, "Bi", "Bismuth", 15, 6}, {84, "Po", "Polonium", 16, 6}, {85, "At", "Astatine", 17, 6}, {86, "Rn", "Radon", 18, 6},
            {87, "Fr", "Francium", 1, 7}, {88, "Ra", "Radium", 2, 7}, {89, "Ac", "Actinium", 3, 7}, {104, "Rf", "Rutherfordium", 4, 7}, {105, "Db", "Dubnium", 5, 7}, {106, "Sg", "Seaborgium", 6, 7}, {107, "Bh", "Bohrium", 7, 7}, {108, "Hs", "Hassium", 8, 7}, {109, "Mt", "Meitnerium", 9, 7}, {110, "Ds", "Darmstadtium", 10, 7}, {111, "Rg", "Roentgenium", 11, 7}, {112, "Cn", "Copernicium", 12, 7}, {113, "Nh", "Nihonium", 13, 7}, {114, "Fl", "Flerovium", 14, 7}, {115, "Mc", "Moscovium", 15, 7}, {116, "Lv", "Livermorium", 16, 7}, {117, "Ts", "Tennessine", 17, 7}, {118, "Og", "Oganesson", 18, 7},
            {58, "Ce", "Cerium", 4, 8}, {59, "Pr", "Praseodymium", 5, 8}, {60, "Nd", "Neodymium", 6, 8}, {61, "Pm", "Promethium", 7, 8}, {62, "Sm", "Samarium", 8, 8}, {63, "Eu", "Europium", 9, 8}, {64, "Gd", "Gadolinium", 10, 8}, {65, "Tb", "Terbium", 11, 8}, {66, "Dy", "Dysprosium", 12, 8}, {67, "Ho", "Holmium", 13, 8}, {68, "Er", "Erbium", 14, 8}, {69, "Tm", "Thulium", 15, 8}, {70, "Yb", "Ytterbium", 16, 8}, {71, "Lu", "Lutetium", 17, 8},
            {90, "Th", "Thorium", 4, 9}, {91, "Pa", "Protactinium", 5, 9}, {92, "U", "Uranium", 6, 9}, {93, "Np", "Neptunium", 7, 9}, {94, "Pu", "Plutonium", 8, 9}, {95, "Am", "Americium", 9, 9}, {96, "Cm", "Curium", 10, 9}, {97, "Bk", "Berkelium", 11, 9}, {98, "Cf", "Californium", 12, 9}, {99, "Es", "Einsteinium", 13, 9}, {100, "Fm", "Fermium", 14, 9}, {101, "Md", "Mendelevium", 15, 9}, {102, "No", "Nobelium", 16, 9}, {103, "Lr", "Lawrencium", 17, 9}
        }};

        const ElementButton* findElementAt(int group, int period)
        {
            for (const ElementButton& element : periodicTable)
            {
                if (element.group == group && element.period == period)
                {
                    return &element;
                }
            }

            return nullptr;
        }

        const char* orbitalTypeLabel(OrbitalType type)
        {
            switch (type)
            {
                case OrbitalType::S:
                    return "s";
                case OrbitalType::P:
                    return "p";
                case OrbitalType::D:
                    return "d";
                case OrbitalType::F:
                    return "f";
            }

            return "?";
        }

        int orbitalGroupKey(int principalQuantumNumber, OrbitalType type)
        {
            return principalQuantumNumber * 10 + static_cast<int>(type);
        }

        struct OrbitalGroup
        {
            int key;
            int principalQuantumNumber;
            OrbitalType type;
            int electronCount;
        };

        std::vector<OrbitalGroup> orbitalGroups(const Atom& atom)
        {
            std::vector<OrbitalGroup> groups;

            for (const Orbital& orbital : atom.orbitals)
            {
                const int key = orbitalGroupKey(orbital.principalQuantumNumber, orbital.type);
                auto existing = std::find_if(
                    groups.begin(),
                    groups.end(),
                    [key](const OrbitalGroup& group)
                    {
                        return group.key == key;
                    });

                if (existing != groups.end())
                {
                    existing->electronCount += orbital.electronCount;
                    continue;
                }

                groups.push_back({
                    key,
                    orbital.principalQuantumNumber,
                    orbital.type,
                    orbital.electronCount});
            }

            return groups;
        }

        std::string orbitalGroupLabel(const OrbitalGroup& group)
        {
            std::string label = std::to_string(group.principalQuantumNumber);
            label += orbitalTypeLabel(group.type);
            label += std::to_string(group.electronCount);
            return label;
        }
    }

    Application::Application()
        : m_window(WindowProperties{})
    {
        std::cout << "Application created.\n";
    }

    void Application::run()
    {
        initialize();

        using Clock = std::chrono::steady_clock;
        auto lastFrameTime = Clock::now();

        while (m_running)
        {
            const auto currentFrameTime = Clock::now();
            const std::chrono::duration<float> elapsedTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            const Timestep timestep(elapsedTime.count());

            runFrame(timestep);

            m_window.onUpdate();

            ++m_frameIndex;

            if (shouldClose())
            {
                m_running = false;
            }
        }

        shutdown();
    }

    void Application::initialize()
    {
        std::cout << "HZL Atom Simulation starting up...\n";

        initializeUi();
        selectElement(m_selectedAtomicNumber);
    }

    void Application::shutdown()
    {
        shutdownUi();
        std::cout << "Application shutting down.\n";
    }

    void Application::runFrame(Timestep timestep)
    {
        update(timestep);
        render();
    }

    void Application::update(Timestep timestep)
    {
        m_atomWorld.update(timestep);
        const bool enableCameraInput = !m_uiInitialized || !ImGui::GetIO().WantCaptureMouse;
        m_renderer.update(timestep, m_window, enableCameraInput);

        if (m_frameIndex == 0)
        {
            std::cout << "First frame dt = "
                      << timestep.milliseconds() << " ms\n";
        }
    }

    void Application::render()
    {
        m_renderer.beginFrame();
        m_renderer.renderAtoms(m_atomWorld.atoms());
        renderUi();
        m_renderer.endFrame();
    }

    void Application::initializeUi()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window.nativeHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        m_uiInitialized = true;
    }

    void Application::shutdownUi()
    {
        if (!m_uiInitialized)
        {
            return;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_uiInitialized = false;
    }

    void Application::renderUi()
    {
        if (!m_uiInitialized)
        {
            return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const Atom* currentAtom = m_atomWorld.atoms().empty() ? nullptr : &m_atomWorld.atoms().front();
        const char* symbol = currentAtom == nullptr ? "" : currentAtom->symbol.c_str();
        const char* name = currentAtom == nullptr ? "No element" : currentAtom->name.c_str();
        const int atomicNumber = currentAtom == nullptr ? 0 : currentAtom->atomicNumber;

        ImGui::SetNextWindowPos({10.0f, 10.0f}, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.72f);
        ImGui::Begin(
            "Element",
            nullptr,
            ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_AlwaysAutoResize
                | ImGuiWindowFlags_NoCollapse);

        ImGui::Text("%d%s  %s", atomicNumber, symbol, name);
        if (ImGui::Button("Choose Element"))
        {
            ImGui::OpenPopup("Periodic Table");
        }

        if (ImGui::BeginPopupModal("Periodic Table", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::BeginTable("periodic_table", 18))
            {
                for (int period = 1; period <= 9; ++period)
                {
                    ImGui::TableNextRow();
                    for (int group = 1; group <= 18; ++group)
                    {
                        ImGui::TableSetColumnIndex(group - 1);
                        const ElementButton* element = findElementAt(group, period);
                        if (element == nullptr)
                        {
                            ImGui::Dummy({34.0f, 28.0f});
                            continue;
                        }

                        ImGui::PushID(element->atomicNumber);
                        if (ImGui::Button(element->symbol, {34.0f, 28.0f}))
                        {
                            selectElement(element->atomicNumber);
                            ImGui::CloseCurrentPopup();
                        }

                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetTooltip("%d %s", element->atomicNumber, element->name);
                        }

                        ImGui::PopID();
                    }
                }

                ImGui::EndTable();
            }

            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        if (currentAtom != nullptr)
        {
            renderOrbitalPanel(*currentAtom);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Application::selectElement(int atomicNumber)
    {
        m_selectedAtomicNumber = atomicNumber;
        m_selectedOrbitalGroupKey = -1;
        m_renderer.setUiHighlightedOrbitalGroup(-1, OrbitalType::S);
        m_atomWorld.setAtom(AtomFactory::createElement(atomicNumber));
    }

    void Application::renderOrbitalPanel(const Atom& atom)
    {
        ImGui::SetNextWindowPos({10.0f, 96.0f}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({180.0f, 360.0f}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.72f);
        ImGui::Begin(
            "Orbitals",
            nullptr,
            ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoCollapse);

        if (m_selectedOrbitalGroupKey >= 0)
        {
            if (ImGui::Button("Clear Selection"))
            {
                m_selectedOrbitalGroupKey = -1;
            }
        }

        ImGui::Separator();

        const std::vector<OrbitalGroup> groups = orbitalGroups(atom);
        int highlightedGroupKey = m_selectedOrbitalGroupKey;
        int highlightedPrincipalQuantumNumber = -1;
        OrbitalType highlightedOrbitalType = OrbitalType::S;

        for (const OrbitalGroup& group : groups)
        {
            const std::string label = orbitalGroupLabel(group);
            const bool isSelected = group.key == m_selectedOrbitalGroupKey;

            ImGui::PushID(group.key);
            if (ImGui::Selectable(label.c_str(), isSelected))
            {
                m_selectedOrbitalGroupKey = isSelected ? -1 : group.key;
            }

            if (ImGui::IsItemHovered())
            {
                highlightedGroupKey = group.key;
                highlightedPrincipalQuantumNumber = group.principalQuantumNumber;
                highlightedOrbitalType = group.type;
            }

            ImGui::PopID();
        }

        ImGui::End();

        if (m_selectedOrbitalGroupKey >= 0)
        {
            for (const OrbitalGroup& group : groups)
            {
                if (group.key == m_selectedOrbitalGroupKey)
                {
                    highlightedPrincipalQuantumNumber = group.principalQuantumNumber;
                    highlightedOrbitalType = group.type;
                    break;
                }
            }
        }

        if (highlightedGroupKey < 0)
        {
            m_renderer.setUiHighlightedOrbitalGroup(-1, OrbitalType::S);
            return;
        }

        m_renderer.setUiHighlightedOrbitalGroup(highlightedPrincipalQuantumNumber, highlightedOrbitalType);
    }

    bool Application::shouldClose() const
    {
        return m_window.shouldClose();
    }
}
