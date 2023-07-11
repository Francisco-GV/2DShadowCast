#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

namespace config
{
    unsigned int nRays = 360;

    /* Window */
    std::string winTitle = "2DShadowCast | C++/SFML";
    unsigned int winWidth = 640; 
    unsigned int winHeight = 360; 
    bool antialiasing = true;    

    /* Color */
    sf::Color bgColor = sf::Color::Black;
    sf::Color wallsColor = sf::Color::White;
    sf::Color raysColor = sf::Color(255, 255, 255, 100);
    sf::Color newWallColor = sf::Color::Yellow;

    /* Predefined walls */
    // Polygons copied from https://ncase.me/sight-and-light/ source code
    std::vector<std::vector<std::vector<float>>> predefinedPolygons {
        // Polygon #1
        {{100,150}, {120,50}},
        {{120,50}, {200,80}},
        {{200,80}, {140,210}},
        {{140,210}, {100,150}},

        // Polygon #2
        {{100,200}, {120,250}},
        {{120,250}, {60,300}},
        {{60,300}, {100,200}},

        // Polygon #3
        {{200,260}, {220,150}},
        {{220,150}, {300,200}},
        {{300,200}, {350,320}},
        {{350,320}, {200,260}},

        // Polygon #4
        {{340,60}, {360,40}},
        {{360,40}, {370,70}},
        {{370,70}, {340,60}},

        // Polygon #5
        {{450,190}, {560,170}},
        {{560,170}, {540,270}},
        {{540,270}, {430,290}},
        {{430,290}, {450,190}},

        // Polygon #6
        {{400,95}, {580,50}},
        {{580,50}, {480,150}},
        {{480,150}, {400,95}}
    };

    // Constants
    constexpr double PI = 3.14159265358979323846264338327950288;
}

#endif