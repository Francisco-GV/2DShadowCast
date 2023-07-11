#ifndef WALL_H
#define WALL_H

#include "SFML/Graphics.hpp"

class Wall
{
public:
    Wall(float a1, float a2, float b1, float b2);
    Wall(sf::Vector2f a, sf::Vector2f b);

    sf::Vector2f getA();
    sf::Vector2f getB();

private:
    sf::Vector2f a;
    sf::Vector2f b;
};

#endif