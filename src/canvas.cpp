#include "canvas.h"
#include "wall.h"

Canvas::Canvas() : window(sf::VideoMode(640, 360), "2DShadowCast | C++/SFML")
{   
    // Polygons copied from https://ncase.me/sight-and-light/ source code
    std::vector<std::vector<std::vector<float>>> polygons {
        {{0,0}, {640,0}},
        {{640,0}, {640,360}},
        {{640,360}, {0,360}},
        {{0,360}, {0,0}},

        // Polygon #1
        {{100,150}, {120,50}},
        {{120,50}, {200,80}},
        {{200,80}, {140,210}},
        {{140,210}, {100,150}},

        // Polygon #2
        {{100,200}, {120,250}},
        {{120,250}, {60,300}},
        {{60,300}, {100,200}},

        // Polygon #3
        {{200,260}, {220,150}},
        {{220,150}, {300,200}},
        {{300,200}, {350,320}},
        {{350,320}, {200,260}},

        // Polygon #4
        {{340,60}, {360,40}},
        {{360,40}, {370,70}},
        {{370,70}, {340,60}},

        // Polygon #5
        {{450,190}, {560,170}},
        {{560,170}, {540,270}},
        {{540,270}, {430,290}},
        {{430,290}, {450,190}},

        // Polygon #6
        {{400,95}, {580,50}},
        {{580,50}, {480,150}},
        {{480,150}, {400,95}}
    };

    for (std::vector<std::vector<float>> v1 : polygons)
    {
        float ax = v1[0][0];
        float ay = v1[0][1];
        float bx = v1[1][0];
        float by = v1[1][1];

        walls.emplace_back(ax, ay, bx, by);
    }
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
