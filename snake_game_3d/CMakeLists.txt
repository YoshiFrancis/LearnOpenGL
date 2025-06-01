cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_library(snake_game_3d snake_game_3d.cpp)

find_package(glfw3 REQUIRED)
target_link_libraries(snake_game_3d PRIVATE glfw)
target_include_directories(snake_game_3d PUBLIC ~/Documents/GLAD/include ${CMAKE_CURRENT_SOURCE_DIR}/helpers)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY	https://github.com/g-truc/glm.git
    GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

target_link_libraries(snake_game_3d PRIVATE glm::glm)

