#ifndef CANVAS_H
#define CANVAS_H

#include "SFML/Graphics.hpp"
#include "wall.h"
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
    std::vector<Wall> walls;
};

#endif // !CANVAS_H