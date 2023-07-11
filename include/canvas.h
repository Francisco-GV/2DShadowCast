#ifndef CANVAS_H
#define CANVAS_H

#include "SFML/Graphics.hpp"
#include "wall.h"
#include "ray.h"
#include "vector"

#define PI 3.14159265358979323846264338327950288

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
};

#endif // !CANVAS_H