#include "wall.h"

Wall::Wall(float a1, float a2, float b1, float b2) : a(a1, a2), b(b1, b2)
{
}

sf::Vector2f Wall::getA()
{
    return a;
}

sf::Vector2f Wall::getB()
{
    return b;
}