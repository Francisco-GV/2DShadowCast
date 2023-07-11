#include "canvas.h"
#include "config.h"

sf::ContextSettings Canvas::createContextSettings()
{
    sf::ContextSettings settings;

    if (config::antialiasing)
    {
        settings.antialiasingLevel = 8u;
    }
    
    return settings;
}

Canvas::Canvas() : window(sf::VideoMode(config::winWidth, config::winHeight), 
        config::winTitle, sf::Style::Titlebar | sf::Style::Close, createContextSettings())
{   
    // Load predefined walls
    for (std::vector<std::vector<float>> v1 : config::predefinedPolygons)
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
    for (int i = 0; i < config::nRays; i++)
    {
        float rad = i * config::PI / 180.f;
        Ray ray(window.getSize().x / 2.f, window.getSize().y / 2.f, rad);

        rays.push_back(ray);
    }

    while (window.isOpen())
    {
        update();
        draw();
    }
}

void Canvas::draw()
{
    window.clear(config::bgColor);

    //Draw walls
    for (Wall& wall : walls)
    {
        sf::Vertex lineLines[]
        {
            sf::Vertex(wall.getA(), config::wallsColor),
            sf::Vertex(wall.getB(), config::wallsColor)
        };

        window.draw(lineLines, 2, sf::Lines);
    }

    for (Ray& ray : rays)
    {
        sf::Vertex rayLines[]
        {
            sf::Vertex(ray.getPosition(), config::raysColor),
            sf::Vertex(ray.getDirection(), config::raysColor)
        };

        window.draw(rayLines, 2, sf::Lines);
    }
    window.display(); 
}

void Canvas::update()
{
    manageEvents();
    updateIntersections();
}

void Canvas::manageEvents()
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

void Canvas::updateIntersections()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    for (Ray& ray : rays)
    {
        // Move rays starting point to mouse position
        ray.setPosition(mousePos.x, mousePos.y);

        std::pair<sf::Vector2f, float>* minimum = nullptr;

        for (Wall& wall : walls)
        {
            std::pair<sf::Vector2f, float>* intersection = ray.calculateIntersection(wall);

            if (intersection != nullptr)
            {
                if (minimum == nullptr)
                {
                    minimum = intersection;
                    continue;
                }
                else
                {
                    if (intersection->second < minimum->second)
                    {
                        minimum = intersection;
                    }
                    else
                    {
                        delete intersection;
                    }
                }
            }
        }

        if (minimum != nullptr)
        {
            ray.setDirectionPoint(minimum->first.x, minimum->first.y);
        }

        delete minimum;
    }
}