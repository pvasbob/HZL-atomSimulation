# HZL Atom Simulation

A modern C++ OpenGL atom visualization project.

This project is currently an educational 3D orbital viewer. It lets you choose an element from a periodic-table UI, inspect its available subshells, rotate the scene, and highlight selected orbitals.

## Current Features

- C++20 / CMake project for Visual Studio 2022
- OpenGL 3.3 rendering through GLFW + GLAD
- GLM math library
- Dear ImGui UI
- 3D orbit camera
- Periodic table element picker
- Orbital selection panel
- Wireframe orbital shell visualization
- Basic `s`, `p`, `d`, and `f` orbital shape approximations
- Rainbow orbital color ordering


## Build Environment

Primary environment:

- Microsoft Visual Studio 2022
- MSVC
- CMake project mode

This folder may also be opened in VS Code for editing, but Visual Studio 2022 is the intended build/debug environment.

## Build Steps

1. Open the project folder in Visual Studio 2022.
2. If dependencies or CMake settings changed, run:
   `Project -> Delete Cache and Reconfigure`
3. Build with:
   `Build -> Rebuild All`
4. Run:
   `HZLAtomSimulation.exe`

The first configure/build may download third-party dependencies through CMake FetchContent.

## Controls

- Left mouse drag: rotate the camera around the atom
- Mouse wheel: zoom in/out
- `Choose Element`: open the periodic table
- Orbitals panel: hover or click an orbital group to highlight it
- `Clear Selection`: remove the locked orbital highlight

## Visualization Notes

The orbital shapes are educational approximations, not exact quantum probability-density surfaces.

Current representation:

- `s`: spherical shell
- `p`: dumbbell shell
- `d`: clover-like shell approximation
- `f`: multi-lobed shell approximation

The element model uses occupied subshells in standard filling order and emphasizes the visible orbital structure rather than rendering every electron as an individual object.

## Current Milestone

`v0.1-orbital-viewer`

The project is now stable enough to serve as a foundation for future atom simulation and visualization work.

## Roadmap

- Improve `d` and `f` orbital geometry
- Add real electron configuration exceptions
- Add exact orbital probability-density shaders
- Add labels and legends for orbital colors
- Add screenshot export
- Add saved camera views
- Add multiple visualization modes:
  - shell mode
  - point cloud mode
  - probability density surface mode

