#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <utility>

#include <SFML/Graphics.hpp>

#include "wall.h"
#include "ray.h"

class Canvas
{
public:
    Canvas();
    void start();
    void draw();
    void update();
private:
    sf::RenderWindow window;
    std::vector<Ray> rays;
    std::vector<Wall> walls;
    std::vector<Wall> boundaryWalls;

    sf::Vector2f mousePosition;
    bool raysVisible = true;
    sf::Vector2f* firstPoint = nullptr;
    Wall* nearestWall = nullptr;

    sf::Cursor clickCursor;
    sf::Cursor defaultCursor;
    bool isCtrlPressed;

    std::pair<Wall*, sf::Vector2i> lookUpNearestWall(sf::Vector2f& point, float maxDistance = 5.f);
    void updateIntersections();
    void manageEvents();
    static sf::ContextSettings createContextSettings();
};

#endif // !CANVAS_H