cmake_minimum_required(VERSION 3.20)
project(PathfindingVisualizer VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(pathfinding_visualizer
    src/main.cpp
    src/Visualizer.cpp
    src/Pathfinder.cpp
)

target_include_directories(pathfinding_visualizer PRIVATE include)
target_link_libraries(pathfinding_visualizer PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(MSVC)
    target_compile_options(pathfinding_visualizer PRIVATE /W4)
else()
    target_compile_options(pathfinding_visualizer PRIVATE -Wall -Wextra -Wpedantic)
endif()
