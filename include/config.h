#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace config
{
    void loadConfigFile(char* argPath);
    extern std::string configFileName;

    extern unsigned int nRays;
    extern bool smartRays;
    extern bool showSmartRays;
    extern float adjacentRaysOffset;
    extern bool showPredefinedPolygons;

    /* Window */
    extern std::string winTitle;
    extern unsigned int winWidth;
    extern unsigned int winHeight;
    extern bool antialiasing;
    extern unsigned int framerateLimit;
    extern bool lightShader;
    extern int shaderRadius;

    /* Color */
    extern sf::Color bgColor;
    extern sf::Color wallsColor;
    extern sf::Color raysColor;
    extern sf::Color newWallColor;
    extern sf::Color nearestWallColor;

    /* Predefined walls */
    // Polygons copied from https://ncase.me/sight-and-light/ source code
    extern std::vector<std::vector<std::vector<float>>> predefinedPolygons;

    /* Shader */
    // Gradient shader from https://github.com/SFML/SFML/wiki/Source:-Radial-Gradient-Shader
    const char RadialGradient[] =
        "uniform vec4 color;"
        "uniform float expand;"
        "uniform vec2 center;"
        "uniform float radius;"
        "uniform float windowHeight;"
        "void main(void)"
        "{"
            "vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);"
            "vec2 p = (centerFromSfml - gl_FragCoord.xy) / radius;"
            "float r = sqrt(dot(p, p));"
            "if (r < 1.0)"
            "{"
                "gl_FragColor = mix(color, gl_Color, (r - expand) / (1 - expand));"
            "}"
            "else"
            "{"
                "gl_FragColor = gl_Color;"
            "}"
        "}";

    const char VertexShader[] =
        "void main()"
        "{"
            "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
            "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
            "gl_FrontColor = gl_Color;"
       "}";

    // Constants
    constexpr double PI = 3.14159265358979323846264338327950288;
}

#endif