# 2DShadowCast
2D Shadow Casting in C++

![window gif](https://github.com/Francisco-GV/2DShadowCast/assets/54688495/2830530c-97f4-40ef-ad94-bfac661d8dd3)

The program simulates the casting of shadows in a 2D environment based on the ray casting algorithm, implemented in C++ using the SFML library.

## Features
+ **Dynamic ray source**: Move the mouse to control the source of the rays.
+ **Interactive wall creation**: 
    + `T` to toggle rays source.
    + `Left-click` to create new walls: **click** to set the starting point, and **click again** elsewhere to set the ending point.
    + `Esc` to cancel new wall.
    + `Ctrl + Left-click` to delete existing wall.
    + `Shift` to snap the cursor to nearest wall (precise placement).

![wall creation](https://github.com/Francisco-GV/2DShadowCast/assets/54688495/c954dd49-77cd-43e8-9457-87c48c06c822)

## Configuration File
You can use a configuration file to modify certain values without the need of modify/recompile the code.  
1. Create a file named `config.txt` in the root of the project (will be copied to build during CMake configuration) or directly in the CMake build directory.
2. Write in each line the values, in the format `<attribute-name>=<value>` e.g. `winWidth=640`

(You can also examine/modify the existing one with the default values)

### Existing attributes (for now)
| Attribute      | Description                           | Default | Type         |
|----------------|---------------------------------------|---------|--------------|
| nRays          | Set number of rays                    | 360     | unsigned int |
| winWidth       | Set width of window                   | 640     | unsigned int |
| winHeight      | Set height of window                  | 360     | unsigned int |
| framerateLimit | Specify FPS limit                     | 60      | unsigned int |
| antialiasing   | Enable antialiasing  (smooth drawing) | true    | bool         |
| showPolygons   | Show predefined polygons at start     | true    | bool         |

## Installation

### Requirements
To compile this project, you need to have installed:
+ [SFML](https://www.sfml-dev.org/index.php)
+ [CMake](https://cmake.org/)
+ [git](https://git-scm.com/) (to clone it)

### Getting Started
To run the program locally, follow these steps:

+ Clone the repository:  

```
git clone https://github.com/Francisco-GV/2DShadowCast.git
```

+ Build the project using CMake:  
```
cd 2DShadowCast
mkdir build
cd build
cmake ..
cmake --build .
```

+ Run the executable:

```
./2DShadowCast
```