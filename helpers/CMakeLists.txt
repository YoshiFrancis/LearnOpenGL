cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(Helpers camera.hpp shader.cpp texture.cpp stb_image.h ~/Documents/GLAD/src/glad.c)
target_include_directories(Helpers PUBLIC ~/Documents/GLAD/include .)


