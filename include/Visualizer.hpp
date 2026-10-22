#pragma once

#include "Pathfinder.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Visualizer {
public:
    Visualizer();
    void run();

private:
    static constexpr int Rows = 25;
    static constexpr int Cols = 40;
    static constexpr float CellSize = 21.f;
    static constexpr float TopBarHeight = 82.f;
    static constexpr float SidePanelWidth = 320.f;
    static constexpr float GridWidth = Cols * CellSize;
    static constexpr float GridHeight = Rows * CellSize;

    sf::RenderWindow window;
    std::vector<bool> walls;
    std::vector<bool> visited;
    std::vector<bool> path;
    int startNode;
    int goalNode;
    Algorithm algorithm;
    bool searchFinished = false;
    bool draggingStart = false;
    bool draggingGoal = false;
    std::string status = "Ready. Draw walls, then run a search.";
    SearchResult lastResult;

    int index(int row, int col) const;
    bool insideGrid(sf::Vector2i mouse) const;
    int nodeAt(sf::Vector2i mouse) const;
    void handleEvents();
    void handleMousePress(sf::Vector2i mouse);
    void runSearch();
    void clearSearch();
    void resetGrid();
    void draw();
    void drawGrid();
    void drawSidebar();
    void drawText(const std::string& text, float x, float y,
                  unsigned size, sf::Color color);
    void drawButton(const sf::FloatRect& rect, const std::string& label,
                    sf::Color fill, sf::Color textColor = sf::Color::White,
                    bool selected = false);
    void drawStatCard(float x, float y, const std::string& label,
                      const std::string& value, sf::Color accent);
    std::string algorithmName() const;
};
