#include "Visualizer.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        Visualizer app;
        app.run();
    } catch (const std::exception& error) {
        std::cerr << "Application error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
