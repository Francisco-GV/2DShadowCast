#include "ray.h"

Ray::Ray(float x, float y, float angle) : startingPoint(x, y), angle(angle)
{
    calculateDirection();
}

Ray::Ray(float x, float y, sf::Vector2f directionPoint) : startingPoint(x, y), direction(directionPoint)
{
    calculateAngle();
}


sf::Vector2f Ray::getPosition()
{
    return startingPoint;
}

sf::Vector2f Ray::getDirection()
{
    return direction;
}

void Ray::setPosition(float x, float y)
{
    startingPoint.x = x;
    startingPoint.y = y;

    calculateDirection();
    calculateAngle();
}

void Ray::setDirectionPoint(float x, float y)
{
    direction.x = x;
    direction.y = y;

    calculateAngle();
}

void Ray::setAngle(float a)
{
    angle = a;

    calculateDirection();
}

void Ray::calculateDirection()
{
    direction.x = startingPoint.x + std::cos(angle);
    direction.y = startingPoint.y + std::sin(angle);
}

void Ray::calculateAngle()
{
    float dx = direction.x - startingPoint.x;
    float dy = direction.y - startingPoint.y;

    angle = std::atan2(dy, dx);
}

// Line-Line Intersection Approach
std::pair<sf::Vector2f, float>* Ray::calculateIntersection(Wall& wall)
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

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

    if ((t > 0 && t < 1) && (u > 0))
    {
        float x = x1 + t * (x2 - x1);
        float y = y1 + t * (y2 - y1);   

        return new std::pair(sf::Vector2f(x, y), u);
    }

    return nullptr;
}