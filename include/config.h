#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

namespace config
{
    void loadConfigFile();
    extern std::string configFileName;

    extern unsigned int nRays;

    /* Window */
    extern std::string winTitle;
    extern unsigned int winWidth;
    extern unsigned int winHeight;
    extern bool antialiasing;
    extern unsigned int framerateLimit;

    /* Color */
    extern sf::Color bgColor;
    extern sf::Color wallsColor;
    extern sf::Color raysColor;
    extern sf::Color newWallColor;
    extern sf::Color nearestWallColor;

    /* Predefined walls */
    // Polygons copied from https://ncase.me/sight-and-light/ source code
    extern std::vector<std::vector<std::vector<float>>> predefinedPolygons;

    // Constants
    constexpr double PI = 3.14159265358979323846264338327950288;
}

#endif  