# Pathfinding Studio

A polished interactive pathfinding visualizer built with **C++17** and **SFML 3**. Explore how BFS, DFS, Dijkstra, and A* search through a grid, compare their results, and build obstacles interactively.

## Features

- **Four algorithms:** Breadth-First Search (BFS), Depth-First Search (DFS), Dijkstra, and A*.
- **Interactive grid:** click cells to add/remove walls; drag the green start and pink goal markers.
- **Results dashboard:** explored nodes, path cost, search time, and selected algorithm.
- **Clear visual states:** walls, explored cells, final path, start, and goal have distinct colors.
- **Keyboard shortcuts:** run, clear, reset, and switch algorithms without reaching for the mouse.
- **Separated architecture:** pathfinding logic is independent from SFML rendering.
- **No bundled dataset or external assets required.** The app tries common Windows fonts automatically.

## Controls

| Action | Control |
|---|---|
| Run search | `Space` or **RUN SEARCH** |
| Choose BFS | `1` |
| Choose DFS | `2` |
| Choose Dijkstra | `3` |
| Choose A* | `4` |
| Clear search visualization (keep walls) | `C` |
| Reset walls and results | `R` |
| Add/remove a wall | Left-click a grid cell |
| Move start or goal | Drag the green or pink cell |

## Requirements

- C++17 compiler
- CMake 3.20+
- Ninja (recommended)
- SFML 3.x (Graphics, Window, System)

## Build on Windows with MSYS2 UCRT64

Install dependencies from the **MSYS2 UCRT64** terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-sfml mingw-w64-ucrt-x86_64-ninja
```

Open PowerShell in this project folder and configure/build (adjust the MSYS2 path if yours differs):

```powershell
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe `
  -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/ninja.exe `
  -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64
cmake --build build
.\build\pathfinding_visualizer.exe
```

### If using your existing custom tool paths

If your CMake, Ninja, and compiler are installed in custom folders, use the paths configured on your machine. For example:

```powershell
& "D:\New folder (6)\ucrt64\bin\cmake.exe" -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=D:/gcc/ucrt64/bin/g++.exe `
  -DCMAKE_MAKE_PROGRAM="D:/New folder (6)/ucrt64/bin/ninja.exe" `
  -DCMAKE_PREFIX_PATH="D:/New folder (6)/ucrt64"
& "D:\New folder (6)\ucrt64\bin\cmake.exe" --build build
$env:Path = "D:\New folder (6)\ucrt64\bin;D:\gcc\ucrt64\bin;$env:Path"
.\build\pathfinding_visualizer.exe
```

## Algorithms at a glance

| Algorithm | Strategy | Finds a shortest path on this grid? |
|---|---|---|
| BFS | FIFO queue | Yes, all moves have equal cost |
| DFS | LIFO stack | Not guaranteed |
| Dijkstra | Minimum-distance priority queue | Yes |
| A* | Distance + Manhattan heuristic | Yes, with this four-directional grid |

## Project structure

```text
PathfindingVisualizer/
├── include/
│   ├── Pathfinder.hpp
│   └── Visualizer.hpp
├── src/
│   ├── main.cpp
│   ├── Pathfinder.cpp
│   └── Visualizer.cpp
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Notes

Search execution is calculated immediately; visited cells and the final path are then displayed together. Search time measures the algorithm's computation, not the rendering time.

## License

MIT
