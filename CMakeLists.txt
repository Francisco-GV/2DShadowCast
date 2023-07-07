cmake_minimum_required(VERSION 3.0.0)
project(2DShadowCast VERSION 0.1.0 LANGUAGES CXX)

set(SOURCES src/main.cpp src/canvas.cpp src/wall.cpp)

add_executable(2DShadowCast ${SOURCES})

target_include_directories(2DShadowCast PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SFML 2 REQUIRED COMPONENTS graphics)

target_link_libraries(2DShadowCast PRIVATE sfml-graphics)
target_include_directories(2DShadowCast PRIVATE ${SFML_INCLUDE_DIR})