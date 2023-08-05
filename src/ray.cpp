#include <iostream>
#include <cmath>

#include "ray.h"
#include "config.h"

Ray::Ray(float x, float y, float angle) : startingPoint(x, y)
{
    setAngle(angle);
}

Ray::Ray(float x, float y, sf::Vector2f directionPoint, bool fixed, float offset) : startingPoint(x, y), fixed(fixed), offset(offset)
{
    setDirectionPoint(directionPoint);
}

sf::Vector2f Ray::getPosition()
{
    return startingPoint;
}

sf::Vector2f Ray::getDirection()
{
    return directionPoint;
}

sf::Vector2f Ray::getIntersectionPoint()
{
    return intersectionPoint;
}

float Ray::getAngle() const
{
    return angle + offset;
}

float Ray::getOffset()
{
    return offset;
}

void Ray::setPosition(float x, float y)
{
    startingPoint.x = x;
    startingPoint.y = y;

    if (fixed)
    {
        calculateDirection();
    }
    else
    {
        if (offset != 0.f)
        {
            calculateFinalPoint();
        }
        else
        {
            calculateAngle();
        }
    }
}

void Ray::setDirectionPoint(sf::Vector2f point)
{
    directionPoint = point;

    if (offset != 0.f)
    {
        calculateFinalPoint();
    }
    else
    {
        finalPoint = directionPoint;
        calculateAngle();
    }
}

void Ray::setAngle(float a)
{
    angle = a;

    calculateDirection();
}

void Ray::setIntersectionPoint(sf::Vector2f point)
{
    intersectionPoint = point;
}

void Ray::calculateDirection()
{
    sf::Vector2f point;

    point.x = startingPoint.x + std::cos(angle);
    point.y = startingPoint.y + std::sin(angle);

    setDirectionPoint(point);
}

void Ray::calculateFinalPoint()
{
    calculateAngle();
    finalPoint.x = startingPoint.x + std::cos(angle + offset);
    finalPoint.y = startingPoint.y + std::sin(angle + offset);
}

void Ray::calculateAngle()
{
    float dx = directionPoint.x - startingPoint.x;
    float dy = directionPoint.y - startingPoint.y;

    angle = std::atan2(dy, dx);
}

// Line-Line Intersection with two line Segments
std::pair<sf::Vector2f, float>* Ray::calculateIntersection(Wall& wall)
{
    float x1 = wall.getA().x;
    float x2 = wall.getB().x;
    float x3 = startingPoint.x;
    float x4 = finalPoint.x;

    float y1 = wall.getA().y;
    float y2 = wall.getB().y;
    float y3 = startingPoint.y;
    float y4 = finalPoint.y;

    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (denominator == 0) // If denominator is 0, the lines are parallel or coincident
    {
        return nullptr;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denominator;

    if ((t >= 0 && t <= 1) && (u > 0))
    {
        float x = x1 + t * (x2 - x1);
        float y = y1 + t * (y2 - y1);   

        return new std::pair(sf::Vector2f(x, y), u);
    }

    return nullptr;
}

bool Ray::operator<(const Ray& ray) const
{
    return getAngle() < ray.getAngle();
}

bool Ray::operator==(const Ray& ray) const
{
    return fabs(getAngle() - ray.getAngle()) <= config::adjacentRaysOffset;
}
