#ifndef RAY_H
#define RAY_H

#include "SFML/Graphics.hpp"
#include "wall.h"
#include <cmath>

class Ray
{
public:
    Ray(float x, float y, float angle);
    Ray(float x, float y, sf::Vector2f directionPoint);
    sf::Vector2f* calculateIntersection(Wall& wall);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    void setDirectionPoint(float x, float y);
    sf::Vector2f getDirection();
    void setAngle(float a);
private:
    float angle; // Radians
    sf::Vector2f startingPoint; 
    sf::Vector2f direction;
    void calculateDirection();
    void calculateAngle();
};

#endif