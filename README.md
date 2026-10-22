# Pathfinding Studio — Interactive Algorithm Visualizer

A C++17 desktop application that visualizes how classic pathfinding algorithms explore a grid and find a route from a start node to a goal node. Built with **SFML 3** and **CMake**.

## Screenshot

![Pathfinding Studio interface](screenshots/pathfinding-studio.png)

## Features

- Interactive grid with start and goal nodes.
- Draw obstacles to create different pathfinding challenges.
- Compare four search algorithms:
  - **Breadth-First Search (BFS)** — finds a shortest path when every move has equal cost.
  - **Depth-First Search (DFS)** — explores deeply before backtracking; it does not guarantee the shortest path.
  - **Dijkstra's Algorithm** — finds a minimum-cost path for non-negative edge weights.
  - **A\*** — uses a heuristic to guide the search toward the goal.
- Results dashboard displaying explored nodes, path cost, search time, and the selected algorithm.
- Clear search results and reset the grid.
- Keyboard shortcuts for common actions.

## Built With

- C++17
- SFML 3
- CMake
- Ninja (optional build generator)

## Getting Started

### Requirements

Install a C++17-compatible compiler, CMake, and SFML 3. Ensure SFML is discoverable by CMake in your environment.

### Build

From the project root, configure and build:

```bash
cmake -S . -B build
cmake --build build
```

Run the executable produced in the `build` directory. The exact executable path depends on your operating system and generator.

### Windows (project's current toolchain)

If you use the same MSYS2/UCRT64 toolchain configuration as the project setup, run these commands in PowerShell from the project root:

```powershell
& "D:\New folder (6)\ucrt64\bin\cmake.exe" -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=D:/gcc/ucrt64/bin/g++.exe -DCMAKE_MAKE_PROGRAM="D:/New folder (6)/ucrt64/bin/ninja.exe" -DCMAKE_PREFIX_PATH="D:/New folder (6)/ucrt64"

& "D:\New folder (6)\ucrt64\bin\cmake.exe" --build build

$env:Path = "D:\New folder (6)\ucrt64\bin;D:\gcc\ucrt64\bin;$env:Path"

.\build\pathfinding_visualizer.exe
```

Update the paths if your compiler, CMake, Ninja, or SFML installation is in a different location.

## How to Use

1. Launch the application.
2. Choose BFS, DFS, Dijkstra, or A* from the algorithm panel.
3. Add walls/obstacles to the grid with the mouse.
4. Press **RUN SEARCH** (or `Space`) to start the search.
5. Review the explored-node count, path cost, execution time, and displayed route.
6. Use **CLEAR** (`C`) to clear the current search, or **RESET** (`R`) to reset the grid.

> Note: Exact mouse interactions and keyboard shortcuts may vary slightly depending on the current source implementation.

## Keyboard Shortcuts

| Key | Action |
|---|---|
| `Space` | Run search |
| `1` | Select BFS |
| `2` | Select DFS |
| `3` | Select Dijkstra |
| `4` | Select A* |
| `C` | Clear search |
| `R` | Reset grid |

## Project Structure

```text
PathfindingVisualizer/
├── include/       # Header files
├── src/           # C++ source files
├── assets/        # Optional application assets
├── screenshots/   # README screenshots
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## What This Project Demonstrates

This project provides a visual way to study graph traversal and pathfinding. It helps compare how different algorithms explore a grid and how many nodes they visit before reaching the goal.

## Future Improvements

- Step-by-step animated search with pause and speed controls.
- Weighted terrain and custom movement costs.
- Random maze generation.
- Side-by-side algorithm comparison.
- Unit tests for pathfinding correctness.

## Author

**Ahmed Osama**

GitHub: [@ahmedosama2321](https://github.com/ahmedosama2321)

---

If you find this project useful, feel free to star the repository.
