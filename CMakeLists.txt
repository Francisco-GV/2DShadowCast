cmake_minimum_required(VERSION 3.0.0)
project(2DShadowCast VERSION 1.1.0 LANGUAGES CXX)

set(SOURCES src/main.cpp src/canvas.cpp src/wall.cpp src/ray.cpp src/config.cpp)
add_executable(2DShadowCast ${SOURCES})
target_include_directories(2DShadowCast PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Setup SFML
set(SFML_LIBS sfml-graphics sfml-window sfml-system)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
target_link_libraries(2DShadowCast PRIVATE ${SFML_LIBS})
target_include_directories(2DShadowCast PRIVATE ${SFML_INCLUDE_DIR})

# Copy SFML DLLs to build directory
foreach (lib ${SFML_LIBS})
    get_target_property(lib_path ${lib} LOCATION)
    file(COPY ${lib_path} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# Copy config.txt to build directory
set(CONFIG_FILE config.txt)
if (EXISTS "${CMAKE_SOURCE_DIR}/${CONFIG_FILE}")
    file(COPY "${CMAKE_SOURCE_DIR}/${CONFIG_FILE}" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()