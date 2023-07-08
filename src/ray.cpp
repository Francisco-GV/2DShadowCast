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