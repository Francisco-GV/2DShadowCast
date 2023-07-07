#include "canvas.h"
#include "wall.h"

Canvas::Canvas() : window(sf::VideoMode(500, 500), "2DShadowCast | C++/SFML")
{
    walls.emplace_back(50, 50, 100, 100);
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

    //Draw walls
    for (Wall wall : walls)
    {
        sf::Vertex lineLines[]
        {
            sf::Vertex(wall.getA(), sf::Color::Black),
            sf::Vertex(wall.getB(), sf::Color::Black)
        };

        window.draw(lineLines, 2, sf::Lines);
    }

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
