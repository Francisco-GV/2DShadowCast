#include "canvas.h"

Canvas::Canvas() : window(sf::VideoMode(500, 500), "2DShadowCast | C++/SFML")
{
    
}

void Canvas::start()
{
    while (window.isOpen())
    {
        update();
        draw();
    }
}

void Canvas::draw()
{
    window.clear(sf::Color::White);

    window.display();
}

void Canvas::update()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}
