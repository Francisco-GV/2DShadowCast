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
    Ray(float x, float y, sf::Vector2f directionPoint, bool fixed, float offset = 0.f);

    std::pair<sf::Vector2f, float>* calculateIntersection(Wall& wall);
    void setPosition(float x, float y);
    void setDirectionPoint(sf::Vector2f point);
    void setAngle(float a);
    void setIntersectionPoint(sf::Vector2f point);
    sf::Vector2f getDirection();
    sf::Vector2f getPosition();
    sf::Vector2f getIntersectionPoint();
    float getAngle() const;
    float getOffset();
    // Operators
    bool operator<(const Ray& ray) const;
    bool operator==(const Ray& ray) const;
private:
    float angle; // Radians
    float offset;
    bool fixed = true;
    sf::Vector2f startingPoint;
    sf::Vector2f directionPoint;
    sf::Vector2f finalPoint;
    sf::Vector2f intersectionPoint;
    void calculateDirection();
    void calculateAngle();
    void calculateFinalPoint();
};

#endif