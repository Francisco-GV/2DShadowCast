#include "SFML/Graphics.hpp"

class Wall
{
public:
    Wall(float a1, float a2, float b1, float b2);

    sf::Vector2f getA();
    sf::Vector2f getB();

private:
    sf::Vector2f a;
    sf::Vector2f b;
};