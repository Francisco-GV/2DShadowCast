#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

#include <SFML/Graphics.hpp>

#include "config.h"

namespace config
{
    namespace // Utility functions
    {
        void trimString(std::string& s, const std::string& characters = " \t\n\r\f\v")
        {
            s.erase(0, s.find_first_not_of(characters));
            s.erase(s.find_last_not_of(characters) + 1);
        }

        bool isNumber(std::string& s)
        {
            return !s.empty() && std::find_if(s.begin(), s.end(), 
                [](unsigned char c) {return !std::isdigit(c);}) == s.end();
        }

        bool validateBool(std::string& s)
        {
            if (s.empty()) return false;
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);

            return (s == "true");
        }

        std::vector<std::string> split(std::string& s, std::string delimiter)
        {
            std::vector<std::string> splitted;

            size_t delimiterSize = delimiter.length();
            size_t start = 0;
            size_t end = 0;

            std::string token;
            while ((end = s.find(delimiter, start)) != std::string::npos)
            {
                token = s.substr(start, end-start);
                trimString(token);
                splitted.push_back(token);
                
                start = end + delimiterSize;
            }
            token = s.substr(start);
            trimString(token);
            splitted.push_back(token);

            return splitted;
        }

        sf::Color parseColor(std::string& s)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            int a = 255;

            //255,255,255
            std::vector<std::string> splitted = split(s, std::string(","));
            size_t size = splitted.size();
            
            if (size == 1 || size == 2)
            {
                for (int i = 0; i < size; i++)
                {
                    if (isNumber(splitted[i]))
                    {
                        int number = std::stoi(splitted[i]);
                        if (number >= 0 && number <= 255)
                        {
                            if (i == 0)
                            {
                                r = number;
                                g = number;
                                b = number;
                            }
                            else if (i == 1) a = number;
                        }
                    }
                }
            }
            else if (size == 3 || size == 4)
            {
                for (int i = 0; i < size; i++)
                {
                    if (isNumber(splitted[i]))
                    {
                        int number = std::stoi(splitted[i]);
                        if (number >= 0 && number <= 255)
                        {
                            if      (i == 0) r = number;
                            else if (i == 1) g = number;
                            else if (i == 2) b = number;
                            else if (i == 3) a = number;
                        }
                    } 
                }
            }

            return sf::Color(r, g, b, a);
        }

        std::string getDirectoryPath(char* path)
        {
            std::string strPath(path);
            
            size_t pos = strPath.find_last_of("\\");
            strPath.erase(pos);

            return strPath;
        }
    }

    void loadConfigFile(char* argPath)
    {
        std::string filePath = getDirectoryPath(argPath) + "\\" + configFileName;

        std::ifstream configFile(filePath);

        if (!configFile)
        {
            std::cerr << "Failed to open "<< configFileName << ". Using default configuration.\n";
            return; 
        }

        std::string line;
        int counter = 0;
        while (std::getline(configFile, line))
        {
            size_t equalSignPos = line.find("=");
            if (equalSignPos != std::string::npos)
            {
                std::string attribute = line.substr(0, equalSignPos);
                std::string value = line.substr(equalSignPos + 1);

                trimString(attribute);
                trimString(value);

                if (attribute == "nRays")               nRays          = isNumber(value) ? std::stoi(value) : nRays;
                else if (attribute == "winWidth")       winWidth       = isNumber(value) ? std::stoi(value) : winWidth;
                else if (attribute == "winHeight")      winHeight      = isNumber(value) ? std::stoi(value) : winHeight;
                else if (attribute == "framerateLimit") framerateLimit = isNumber(value) ? std::stoi(value) : framerateLimit;
                else if (attribute == "smartRays")      smartRays      = validateBool(value);
                else if (attribute == "showSmartRays")  showSmartRays  = validateBool(value);
                else if (attribute == "antialiasing")   antialiasing   = validateBool(value);
                else if (attribute == "showPolygons")   showPredefinedPolygons = validateBool(value);
                else if (attribute == "bgColor")        bgColor        = parseColor(value);
                else if (attribute == "wallsColor")     wallsColor     = parseColor(value);
                else if (attribute == "raysColor")      raysColor      = parseColor(value);
                else if (attribute == "newWallColor")   newWallColor   = parseColor(value);
                else if (attribute == "nearestWallColor") nearestWallColor = parseColor(value);
            }
        }
    }

    /****************************** DEFAULT VALUES ******************************/

    std::string configFileName = "config.txt";

    unsigned int nRays = 360;
    bool smartRays = false;
    bool showSmartRays = false;
    float adjacentRaysOffset = 0.0001f;
    bool showPredefinedPolygons = true;

    /* Window */
    std::string winTitle = "2DShadowCast | C++/SFML";
    unsigned int winWidth = 640;
    unsigned int winHeight = 360;
    bool antialiasing = true;
    unsigned int framerateLimit = 60;

    /* Color */
    sf::Color bgColor = sf::Color::Black;
    sf::Color wallsColor = sf::Color::White;
    sf::Color raysColor = sf::Color(255, 255, 255, 100);
    sf::Color newWallColor = sf::Color::Yellow;
    sf::Color nearestWallColor = sf::Color::Red;

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
}