# 2DShadowCast
2D Shadow Casting in C++

![window gif](https://github.com/Francisco-GV/2DShadowCast/assets/54688495/2830530c-97f4-40ef-ad94-bfac661d8dd3)

The program simulates the casting of shadows in a 2D environment based on the ray casting algorithm, implemented in C++ using the SFML library.

## Features
+ **Dynamic ray source**: Move the mouse to control the source of the rays.
+ **Interactive wall creation**: Left-click once to set the starting point, and click again to set the ending point, creating a wall segment.
    ![wall creation](https://github.com/Francisco-GV/2DShadowCast/assets/54688495/c954dd49-77cd-43e8-9457-87c48c06c822)


## Installation

### Requirements
To compile this project, you need to have installed:
+ [SFML](https://www.sfml-dev.org/index.php)
+ [CMake](https://cmake.org/)
+ [git](https://git-scm.com/)

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