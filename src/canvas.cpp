#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>

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
    if (config::smartRays)
    {
        for (Wall wall : walls)
        {
            createSmartRays(wall);
        }
    }
    else
    {
        float angleStep = 360.f / config::nRays;
        for (float i = 0; i < 360.f; i += angleStep)
        {
            float rad = i * config::PI / 180.f;
            Ray ray(window.getSize().x / 2.f, window.getSize().y / 2.f, rad);

            rays.push_back(ray);
        }
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
        if (config::smartRays)
        {
            size_t size = optimizedRays.size() + 2;

            sf::Vertex vertices[size];

            vertices[0] = sf::Vertex(mousePosition, config::raysColor);

            for (int i = 0; i < size - 2; i++)
            {
                vertices[i + 1] = sf::Vertex(optimizedRays[i].getIntersectionPoint(), config::raysColor);
            }

            if (optimizedRays.size() != 0) vertices[size - 1] = vertices[1];

            window.draw(vertices, size, sf::TriangleFan);
        }
        
        if (!config::smartRays || config::showSmartRays)
        {
            for (Ray& ray : (config::smartRays) ? optimizedRays : rays)
            {
                sf::Vertex rayLines[]
                {
                    sf::Vertex(ray.getPosition(), config::raysColor),
                    sf::Vertex(ray.getIntersectionPoint(), config::raysColor)
                };
                window.draw(rayLines, 2, sf::Lines);
            }
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
        if (config::smartRays) updateSmartRays();
    }
}

void Canvas::updateSmartRays()
{
    std::sort(rays.begin(), rays.end());
    optimizeSmartRays();
}

void Canvas::optimizeSmartRays()
    { 
    optimizedRays.clear();

    std::unique_copy(rays.begin(), rays.end(), std::back_inserter(optimizedRays));
}

void Canvas::createSmartRays(Wall &wall)
{
    float offset = config::adjacentRaysOffset;

    // Create three rays for each point, two of which have a small offset
    rays.push_back(Ray(0, 0, wall.getA(), false, -offset));
    rays.push_back(Ray(0, 0, wall.getB(), false, -offset));

    rays.push_back(Ray(0, 0, wall.getA(), false));
    rays.push_back(Ray(0, 0, wall.getB(), false));

    rays.push_back(Ray(0, 0, wall.getA(), false, offset));
    rays.push_back(Ray(0, 0, wall.getB(), false, offset));
}

void Canvas::addWall(sf::Vector2f& a, sf::Vector2f& b)
{
    Wall newWall = walls.emplace_back(a, b);

    createSmartRays(newWall);
}

void Canvas::deleteWall(Wall& wall)
{
    sf::Vector2f a = wall.getA();
    sf::Vector2f b = wall.getB();

    // Delete the first three rays associated to each wall point
    for (sf::Vector2f point : std::vector<sf::Vector2f> {a, b})
    {
        for (int i = -1; i <= 1; i++)
        {
            float offset = config::adjacentRaysOffset * i;

            auto predicate = [&point, &offset](Ray& ray) { 
                return ray.getDirection() == point && ray.getOffset() == offset;
            };

            std::vector<Ray>::iterator it = std::find_if(rays.begin(), rays.end(), predicate);
            
            if (it != rays.end())
            {
                it = rays.erase(it);
            }
        }
    }

    // Delete the wall from the vector
    walls.erase(std::remove(walls.begin(), walls.end(), wall), walls.end());
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
                            deleteWall(*nearestWall);
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
                            addWall(*firstPoint, mousePosition);        

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
                            nearestWall = lookUpNearestWall(mousePosition).first;

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
                    
                    case sf::Keyboard::LShift:
                    case sf::Keyboard::RShift:
                        raysVisible = false;
                        {
                            std::pair<Wall*, sf::Vector2i> pair = lookUpNearestWall(mousePosition);
                            Wall* wall = pair.first;
                            
                            if (wall != nullptr)
                            {
                                sf::Mouse::setPosition(pair.second, window);
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
                    
                    case sf::Keyboard::LShift:
                    case sf::Keyboard::RShift:
                        raysVisible = true;
                }
        }
    }
}

std::pair<Wall*, sf::Vector2i> Canvas::lookUpNearestWall(sf::Vector2f& point, float maxDistance)
{
    float x0 = point.x;
    float y0 = point.y;

    float minDistance = std::numeric_limits<float>::max();
    Wall* minWall = nullptr;
    float minT = std::numeric_limits<float>::max();

    // Determine what is the wall with the nearest distance
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
            minT = t;
        }
    }

    sf::Vector2i intersectionPoint;
    
    if (minDistance <= maxDistance)
    {
        float x1 = minWall->getA().x;
        float x2 = minWall->getB().x;
        float y1 = minWall->getA().y;
        float y2 = minWall->getB().y;

        intersectionPoint.x = std::round(x1 + minT * (x2 - x1));
        intersectionPoint.y = std::round(y1 + minT * (y2 - y1));
    }
    else
    {
        minWall = nullptr;
    }

    return std::pair<Wall*, sf::Vector2i>(minWall, intersectionPoint);
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
            ray.setIntersectionPoint(minimum->first);
        }

        delete minimum;
    }
}