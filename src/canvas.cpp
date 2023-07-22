#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>

#include <SFML/Graphics.hpp>

#include "canvas.h"
#include "config.h"
#include "wall.h"
#include "ray.h"

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
    window.setFramerateLimit(config::framerateLimit);

    clickCursor.loadFromSystem(sf::Cursor::Hand);
    defaultCursor.loadFromSystem(sf::Cursor::Arrow);
    isCtrlPressed = false;

    // Load predefined walls
    if (config::showPredefinedPolygons)
    {
        for (std::vector<std::vector<float>> v1 : config::predefinedPolygons)
        {
            float ax = v1[0][0];
            float ay = v1[0][1];
            float bx = v1[1][0];
            float by = v1[1][1];

            walls.emplace_back(ax, ay, bx, by);
        }
    }

    // Create boundary walls
    sf::Vector2f size(window.getSize());

    float boundary[4][2][2] = {
        {{     0,      0}, {size.x,      0}},
        {{size.x,      0}, {size.x, size.y}},
        {{size.x, size.y}, {     0, size.y}},
        {{     0, size.y}, {     0,      0}}
    };

    for (float(&a1)[2][2] : boundary)
    {
        float ax = a1[0][0];
        float ay = a1[0][1];
        float bx = a1[1][0];
        float by = a1[1][1];

        Wall wall = boundaryWalls.emplace_back(ax, ay, bx, by);
        walls.push_back(wall);
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

    // Draw nearest wall when Ctrl is pressed 
    if (nearestWall != nullptr)
    {
        sf::Vertex lineLines[]
        {
            sf::Vertex(nearestWall->getA(), config::nearestWallColor),
            sf::Vertex(nearestWall->getB(), config::nearestWallColor)

        };
        // Over drawn
        window.draw(lineLines, 2, sf::Lines);
    }

    // Draw rays
    if (raysVisible)
    {
        for (Ray& ray : rays)
        {
            sf::Vertex rayLines[]
            {
                sf::Vertex(ray.getPosition(), config::raysColor),
                sf::Vertex(ray.getDirection(), config::raysColor)
            };

            window.draw(rayLines, 2, sf::Lines);
        }
    }

    // Draw new wall
    if (firstPoint != nullptr)
    {   
        sf::Vertex newWallLines[]
        {
            sf::Vertex(*firstPoint, config::newWallColor),
            sf::Vertex(mousePosition, config::newWallColor)
        };

        sf::CircleShape dot(2);
        dot.setFillColor(config::newWallColor);

        sf::Vector2f dotPosition(firstPoint->x - dot.getRadius(), firstPoint->y - dot.getRadius());

        dot.setPosition(dotPosition);
        window.draw(dot);

        dotPosition.x = mousePosition.x - dot.getRadius();
        dotPosition.y = mousePosition.y - dot.getRadius();

        dot.setPosition(dotPosition);
        window.draw(dot);

        window.draw(newWallLines, 2, sf::Lines);
    }

    window.display(); 
}

void Canvas::update()
{
    mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

    manageEvents();

    if (raysVisible)
    {
        updateIntersections();
    }
}

void Canvas::manageEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {   
        switch (event.type)
        {
            // Window
            case sf::Event::Closed:
                window.close();
                break;
            
            // Mouse
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (isCtrlPressed)
                    {
                        if (nearestWall != nullptr)
                        {
                            walls.erase(std::remove(walls.begin(), walls.end(), *nearestWall), walls.end());
                            nearestWall = nullptr;
                        }
                    } 
                    else
                    {
                        if (firstPoint == nullptr)
                        {
                            raysVisible = false;
                            firstPoint = new sf::Vector2f(mousePosition);
                            window.setMouseCursor(clickCursor);
                        }
                        else // if firstPoint != nullptr
                        {
                            raysVisible = true;
                            walls.emplace_back(*firstPoint, mousePosition);                

                            delete firstPoint;
                            firstPoint = nullptr;
                            
                            window.setMouseCursor(defaultCursor);
                        }
                    }                    
                }
                break;

            // Keyboard
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        raysVisible = true;
                        
                        delete firstPoint;
                        firstPoint = nullptr;
                        break;
                        
                    case sf::Keyboard::RControl:
                    case sf::Keyboard::LControl:
                        isCtrlPressed = true;
                        if (firstPoint == nullptr)
                        {
                            nearestWall = lookUpNearestWall(mousePosition);

                            if (nearestWall != nullptr)
                            {
                                window.setMouseCursor(clickCursor);
                            }
                            else
                            {
                                window.setMouseCursor(defaultCursor);
                            }
                        }
                        break;
                    case sf::Keyboard::T: // Toggle
                        raysVisible =! raysVisible;
                        break;
                }
                break;

            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                    case sf::Keyboard::RControl:
                    case sf::Keyboard::LControl:
                        isCtrlPressed = false;
                        nearestWall = nullptr;
                        window.setMouseCursor(defaultCursor);
                        break;
                }
        }
    }
}

Wall* Canvas::lookUpNearestWall(sf::Vector2f& point, float maxDistance)
{
    float x0 = point.x;
    float y0 = point.y;

    float minDistance = std::numeric_limits<float>::max();
    Wall* minWall = nullptr;

    float mint = minDistance;


    for (Wall& wall : walls)
    {
        bool isBoundary = std::find(boundaryWalls.begin(), boundaryWalls.end(), wall) != boundaryWalls.end(); 
        if (isBoundary) continue; // Ignore bounds

        float x1 = wall.getA().x;
        float y1 = wall.getA().y;
        float x2 = wall.getB().x;
        float y2 = wall.getB().y;
        
        float lengthSquared = powf(x2 - x1, 2) + powf(y2 - y1, 2);

        // Normalized distance along the line (wall) segment
        float t = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / lengthSquared;

        // If it's less than 0 and greater than 1, it's beyond the line
        if (t < 0 || t > 1) continue;

        float distance = std::abs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1)) /
                            sqrt(lengthSquared);

        if (distance < minDistance)
        {
            minDistance = distance;
            minWall = &wall;
        }
    }

    if (minDistance > maxDistance)
    {
        minWall = nullptr;
    }

    return minWall;

}

void Canvas::updateIntersections()
{
    for (Ray& ray : rays)
    {
        // Move rays starting point to mouse position
        ray.setPosition(mousePosition.x, mousePosition.y);

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