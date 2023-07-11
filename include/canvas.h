#ifndef CANVAS_H
#define CANVAS_H

#include "SFML/Graphics.hpp"
#include "wall.h"
#include "ray.h"
#include "vector"

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
    sf::Vector2f mousePosition;
    bool raysVisible = true;
    sf::Vector2f* firstPoint = nullptr;


    void updateIntersections();
    void manageEvents();
    static sf::ContextSettings createContextSettings();  
};

#endif // !CANVAS_H