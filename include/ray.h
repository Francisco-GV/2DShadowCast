#ifndef RAY_H
#define RAY_H

#include "SFML/Graphics.hpp"
#include "wall.h"

class Ray
{
public:
    Ray(float x, float y, float angle);
    sf::Vector2f getPosition();
    sf::Vector2f* calculateIntersection(Wall& wall);
private:
    float angle; // Radians
    sf::Vector2f startingPoint;
    sf::Vector2f direction;
    void calculateDirection();
};

#endif