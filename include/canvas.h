#include "SFML/Graphics.hpp"

#ifndef CANVAS_H
#define CANVAS_H

class Canvas
{
public:
    Canvas();
    void start();
    void draw();
    void update();
private:
    sf::RenderWindow window;
};

#endif // !CANVAS_H