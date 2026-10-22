#include "Visualizer.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace {
const sf::Color Background(10, 15, 27);
const sf::Color Panel(17, 25, 42);
const sf::Color PanelRaised(24, 35, 56);
const sf::Color GridLine(30, 43, 64);
const sf::Color TextPrimary(235, 242, 252);
const sf::Color TextMuted(137, 154, 178);
const sf::Color Accent(56, 189, 248);
const sf::Color AccentDark(14, 116, 180);
const sf::Color WallColor(79, 93, 116);
const sf::Color VisitedColor(39, 93, 155);
const sf::Color PathColor(250, 204, 21);
const sf::Color StartColor(52, 211, 153);
const sf::Color GoalColor(251, 113, 133);
}

Visualizer::Visualizer()
    : window(
          sf::VideoMode({
              static_cast<unsigned>(GridWidth + SidePanelWidth),
              static_cast<unsigned>(TopBarHeight + GridHeight + 44.f)
          }),
          "Pathfinding Studio | Interactive Algorithm Visualizer",
          sf::Style::Titlebar | sf::Style::Close
      ),
      walls(Rows * Cols, false),
      visited(Rows * Cols, false),
      path(Rows * Cols, false),
      startNode(index(Rows / 2, 4)),
      goalNode(index(Rows / 2, Cols - 5)),
      algorithm(Algorithm::AStar) {
    window.setFramerateLimit(60);
}

int Visualizer::index(int row, int col) const { return row * Cols + col; }

bool Visualizer::insideGrid(sf::Vector2i mouse) const {
    return mouse.x >= 0 &&
           mouse.y >= static_cast<int>(TopBarHeight) &&
           mouse.x < static_cast<int>(GridWidth) &&
           mouse.y < static_cast<int>(TopBarHeight + GridHeight);
}

int Visualizer::nodeAt(sf::Vector2i mouse) const {
    if (!insideGrid(mouse)) return -1;
    const int col = mouse.x / static_cast<int>(CellSize);
    const int row = (mouse.y - static_cast<int>(TopBarHeight)) /
                    static_cast<int>(CellSize);
    if (row < 0 || row >= Rows || col < 0 || col >= Cols) return -1;
    return index(row, col);
}

void Visualizer::run() {
    while (window.isOpen()) {
        handleEvents();
        draw();
    }
}

void Visualizer::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (pressed->button == sf::Mouse::Button::Left) handleMousePress(pressed->position);
        } else if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (released->button == sf::Mouse::Button::Left) {
                draggingStart = false;
                draggingGoal = false;
            }
        } else if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
            const sf::Vector2i mouse = moved->position;
            const int node = nodeAt(mouse);
            if (node >= 0 && (draggingStart || draggingGoal)) {
                if (!walls[node] && node != (draggingStart ? goalNode : startNode)) {
                    if (draggingStart) startNode = node;
                    if (draggingGoal) goalNode = node;
                    clearSearch();
                }
            } else if (node >= 0 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (node != startNode && node != goalNode && !walls[node]) {
                    walls[node] = true;
                    clearSearch();
                }
            }
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Space: runSearch(); break;
                case sf::Keyboard::Key::C: clearSearch(); break;
                case sf::Keyboard::Key::R: resetGrid(); break;
                case sf::Keyboard::Key::Num1: algorithm = Algorithm::BFS; clearSearch(); break;
                case sf::Keyboard::Key::Num2: algorithm = Algorithm::DFS; clearSearch(); break;
                case sf::Keyboard::Key::Num3: algorithm = Algorithm::Dijkstra; clearSearch(); break;
                case sf::Keyboard::Key::Num4: algorithm = Algorithm::AStar; clearSearch(); break;
                default: break;
            }
        }
    }
}

void Visualizer::handleMousePress(sf::Vector2i mouse) {
    const int node = nodeAt(mouse);
    if (node >= 0) {
        if (node == startNode) draggingStart = true;
        else if (node == goalNode) draggingGoal = true;
        else {
            walls[node] = !walls[node];
            clearSearch();
        }
        return;
    }

    const float x = GridWidth + 24.f;
    const float mx = static_cast<float>(mouse.x);
    const float my = static_cast<float>(mouse.y);
    if (mx < x || mx > GridWidth + SidePanelWidth - 24.f) return;

    if (my >= 136.f && my <= 172.f) { algorithm = Algorithm::BFS; clearSearch(); }
    else if (my >= 180.f && my <= 216.f) { algorithm = Algorithm::DFS; clearSearch(); }
    else if (my >= 224.f && my <= 260.f) { algorithm = Algorithm::Dijkstra; clearSearch(); }
    else if (my >= 268.f && my <= 304.f) { algorithm = Algorithm::AStar; clearSearch(); }
    else if (my >= 326.f && my <= 370.f) runSearch();
    else if (my >= 382.f && my <= 420.f) clearSearch();
    else if (my >= 430.f && my <= 468.f) resetGrid();
}

void Visualizer::runSearch() {
    clearSearch();
    lastResult = Pathfinder::search(Rows, Cols, walls, startNode, goalNode, algorithm);
    for (int node : lastResult.visitedOrder) visited[node] = true;
    for (int node : lastResult.path) path[node] = true;
    searchFinished = true;

    std::ostringstream out;
    if (lastResult.found) {
        out << algorithmName() << " completed  |  "
            << lastResult.pathCost << " steps  |  "
            << lastResult.exploredCount << " nodes explored";
        status = out.str();
    } else {
        status = algorithmName() + " couldn't reach the goal. Try clearing a few walls.";
    }
}

void Visualizer::clearSearch() {
    std::fill(visited.begin(), visited.end(), false);
    std::fill(path.begin(), path.end(), false);
    searchFinished = false;
    lastResult = SearchResult{};
    status = "Search cleared. Ready when you are.";
}

void Visualizer::resetGrid() {
    std::fill(walls.begin(), walls.end(), false);
    clearSearch();
    status = "Grid reset. Start and goal are back on an open grid.";
}

std::string Visualizer::algorithmName() const {
    switch (algorithm) {
        case Algorithm::BFS: return "BFS";
        case Algorithm::DFS: return "DFS";
        case Algorithm::Dijkstra: return "Dijkstra";
        case Algorithm::AStar: return "A*";
    }
    return "Unknown";
}

void Visualizer::drawText(const std::string& text, float x, float y,
                          unsigned size, sf::Color color) {
    static sf::Font font;
    static bool loaded = false;
    static const char* candidates[] = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "assets/DejaVuSans.ttf"
    };
    if (!loaded) {
        for (const char* candidate : candidates) {
            if (font.openFromFile(candidate)) { loaded = true; break; }
        }
    }
    if (!loaded) return;
    sf::Text label(font, text, size);
    label.setFillColor(color);
    label.setPosition({x, y});
    window.draw(label);
}

void Visualizer::drawButton(const sf::FloatRect& rect, const std::string& label,
                            sf::Color fill, sf::Color textColor, bool selected) {
    sf::RectangleShape button(rect.size);
    button.setPosition(rect.position);
    button.setFillColor(fill);
    button.setOutlineThickness(selected ? 2.f : 1.f);
    button.setOutlineColor(selected ? Accent : sf::Color(42, 57, 80));
    window.draw(button);
    drawText(label, rect.position.x + 13.f, rect.position.y + 8.f, 14, textColor);
}

void Visualizer::drawStatCard(float x, float y, const std::string& label,
                              const std::string& value, sf::Color accent) {
    sf::RectangleShape card({126.f, 57.f});
    card.setPosition({x, y});
    card.setFillColor(PanelRaised);
    card.setOutlineThickness(1.f);
    card.setOutlineColor(sf::Color(36, 51, 74));
    window.draw(card);
    sf::RectangleShape marker({3.f, 34.f});
    marker.setPosition({x + 10.f, y + 11.f});
    marker.setFillColor(accent);
    window.draw(marker);
    drawText(label, x + 20.f, y + 8.f, 10, TextMuted);
    drawText(value, x + 20.f, y + 27.f, 16, TextPrimary);
}

void Visualizer::drawGrid() {
    for (int r = 0; r < Rows; ++r) {
        for (int c = 0; c < Cols; ++c) {
            const int node = index(r, c);
            sf::RectangleShape cell({CellSize - 1.f, CellSize - 1.f});
            cell.setPosition({c * CellSize, TopBarHeight + r * CellSize});
            sf::Color color(21, 31, 49);
            if (walls[node]) color = WallColor;
            if (visited[node]) color = VisitedColor;
            if (path[node]) color = PathColor;
            if (node == startNode) color = StartColor;
            if (node == goalNode) color = GoalColor;
            cell.setFillColor(color);
            cell.setOutlineThickness(0.5f);
            cell.setOutlineColor(GridLine);
            window.draw(cell);
        }
    }
}

void Visualizer::drawSidebar() {
    const float x = GridWidth + 24.f;
    drawText("WORKSPACE", x, 99.f, 11, Accent);
    drawText("Choose an algorithm", x, 115.f, 17, TextPrimary);

    drawButton({{x, 136.f}, {272.f, 36.f}}, "1   BFS     ·  shortest path", 
               algorithm == Algorithm::BFS ? sf::Color(30, 75, 101) : PanelRaised,
               TextPrimary, algorithm == Algorithm::BFS);
    drawButton({{x, 180.f}, {272.f, 36.f}}, "2   DFS     ·  depth first", 
               algorithm == Algorithm::DFS ? sf::Color(30, 75, 101) : PanelRaised,
               TextPrimary, algorithm == Algorithm::DFS);
    drawButton({{x, 224.f}, {272.f, 36.f}}, "3   Dijkstra ·  weighted search", 
               algorithm == Algorithm::Dijkstra ? sf::Color(30, 75, 101) : PanelRaised,
               TextPrimary, algorithm == Algorithm::Dijkstra);
    drawButton({{x, 268.f}, {272.f, 36.f}}, "4   A*       ·  heuristic search", 
               algorithm == Algorithm::AStar ? sf::Color(30, 75, 101) : PanelRaised,
               TextPrimary, algorithm == Algorithm::AStar);

    drawButton({{x, 326.f}, {272.f, 44.f}}, "▶   RUN SEARCH     [SPACE]", AccentDark, TextPrimary, true);
    drawButton({{x, 382.f}, {130.f, 38.f}}, "CLEAR  [C]", PanelRaised, TextPrimary);
    drawButton({{x + 142.f, 382.f}, {130.f, 38.f}}, "RESET  [R]", PanelRaised, TextPrimary);

    drawText("LIVE RESULTS", x, 442.f, 11, Accent);
    drawText(searchFinished ? (lastResult.found ? "Path found" : "No path found") : "Waiting for a search",
             x, 459.f, 16, searchFinished && lastResult.found ? StartColor :
             (searchFinished ? GoalColor : TextMuted));

    std::ostringstream nodes, cost, time;
    nodes << (searchFinished ? std::to_string(lastResult.exploredCount) : "--");
    cost << (searchFinished && lastResult.found ? std::to_string(lastResult.pathCost) : "--");
    time << std::fixed << std::setprecision(2)
         << (searchFinished ? lastResult.executionTimeMs : 0.0);
    drawStatCard(x, 484.f, "NODES EXPLORED", nodes.str(), Accent);
    drawStatCard(x + 142.f, 484.f, "PATH COST", cost.str(), PathColor);
    drawStatCard(x, 551.f, "SEARCH TIME", searchFinished ? time.str() + " ms" : "--", StartColor);
    drawStatCard(x + 142.f, 551.f, "ALGORITHM", algorithmName(), GoalColor);

    drawText("CELL LEGEND", x, 620.f, 11, TextMuted);
    const sf::Color colors[] = {StartColor, GoalColor, WallColor, VisitedColor, PathColor};
    const std::string labels[] = {"Start", "Goal", "Wall", "Visited", "Path"};
    const float positions[] = {x, x + 58.f, x + 116.f, x + 184.f, x + 250.f};
    for (int i = 0; i < 5; ++i) {
        sf::RectangleShape swatch({9.f, 9.f});
        swatch.setPosition({positions[i], 643.f});
        swatch.setFillColor(colors[i]);
        window.draw(swatch);
        drawText(labels[i], positions[i] + 12.f, 639.f, 10, TextMuted);
    }
}

void Visualizer::draw() {
    window.clear(Background);

    sf::RectangleShape header({static_cast<float>(window.getSize().x), TopBarHeight});
    header.setFillColor(Panel);
    window.draw(header);

    sf::RectangleShape accentLine({static_cast<float>(window.getSize().x), 2.f});
    accentLine.setPosition({0.f, TopBarHeight - 2.f});
    accentLine.setFillColor(Accent);
    window.draw(accentLine);

    drawText("PATHFINDING", 22.f, 13.f, 22, TextPrimary);
    drawText("STUDIO", 22.f, 40.f, 22, Accent);
    drawText("Explore how search algorithms navigate a grid.", 202.f, 30.f, 14, TextMuted);
    drawText("C++17  /  SFML 3", GridWidth + 24.f, 22.f, 12, TextMuted);
    drawText("INTERACTIVE DEMO", GridWidth + 24.f, 43.f, 11, Accent);

    sf::RectangleShape sidebar({SidePanelWidth, static_cast<float>(window.getSize().y) - TopBarHeight});
    sidebar.setPosition({GridWidth, TopBarHeight});
    sidebar.setFillColor(Panel);
    window.draw(sidebar);

    drawGrid();

    sf::RectangleShape footer({GridWidth, 44.f});
    footer.setPosition({0.f, TopBarHeight + GridHeight});
    footer.setFillColor(Panel);
    window.draw(footer);
    drawText("HOW TO USE", 14.f, TopBarHeight + GridHeight + 5.f, 10, Accent);
    drawText("Click cells: add/remove walls   •   Drag START / GOAL to move endpoints",
             14.f, TopBarHeight + GridHeight + 20.f, 11, TextMuted);
    drawSidebar();

    // A narrow divider separates the grid from the controls.
    sf::RectangleShape divider({1.f, static_cast<float>(window.getSize().y) - TopBarHeight});
    divider.setPosition({GridWidth, TopBarHeight});
    divider.setFillColor(sf::Color(40, 56, 79));
    window.draw(divider);

    // Keep status text visible without obscuring the grid.
    if (!status.empty()) {
        sf::RectangleShape statusBar({GridWidth, 20.f});
        statusBar.setPosition({0.f, TopBarHeight + GridHeight - 20.f});
        statusBar.setFillColor(sf::Color(13, 22, 37, 235));
        window.draw(statusBar);
        drawText(status, 10.f, TopBarHeight + GridHeight - 18.f, 10,
                 searchFinished && !lastResult.found ? GoalColor : TextMuted);
    }
    window.display();
}
