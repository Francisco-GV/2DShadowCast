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
    void updateIntersections();
private:
    sf::RenderWindow window;
    std::vector<Ray> rays;
    std::vector<Wall> walls;

    static sf::ContextSettings createContextSettings();
};

#endif // !CANVAS_H