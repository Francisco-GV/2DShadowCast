#include "ray.h"

#include <cmath>

Ray::Ray(float x, float y, float angle) : startingPoint(x, y), angle(angle)
{
}

sf::Vector2f Ray::getPosition()
{
    return startingPoint;
}

void Ray::calculateDirection()
{
    direction.x = startingPoint.x + std::cos(angle);
    direction.y = startingPoint.y + std::sin(angle);
}

// Line-Line Intersection Approach
sf::Vector2f* Ray::calculateIntersection(Wall& wall)
{
    float x1 = wall.getA().x;
    float x2 = wall.getB().x;
    float x3 = startingPoint.x;
    float x4 = direction.x;

    float y1 = wall.getA().y;
    float y2 = wall.getB().y;
    float y3 = startingPoint.y;
    float y4 = direction.y;

    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    float t = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4) / denominator;
    float u = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2) / denominator;

    if ((t > 0 && t < 1) && (u > 0))
    {
        float x = x1 + t * (x2 - x1);
        float y = y1 + t * (y2 - y1);   

        return new sf::Vector2f(x, y);
    }

    return nullptr;
}