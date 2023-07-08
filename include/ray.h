#ifndef RAY_H
#define RAY_H

#include "SFML/Graphics.hpp"

class Ray
{
public:
    Ray(float x, float y, float angle);
    sf::Vector2f getPosition();
private:
    float angle; // Radians
    sf::Vector2f startingPoint;
    sf::Vector2f direction;
    void calculateDirection();
};

#endif