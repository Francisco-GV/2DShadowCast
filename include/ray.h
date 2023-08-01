#ifndef RAY_H
#define RAY_H

#include "SFML/Graphics.hpp"
#include "wall.h"
#include <utility>
#include <cmath>

class Ray
{
public:
    Ray(float x, float y, float angle);
    Ray(float x, float y, sf::Vector2f directionPoint);
    std::pair<sf::Vector2f, float>* calculateIntersection(Wall& wall);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    void setDirectionPoint(sf::Vector2f point);
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