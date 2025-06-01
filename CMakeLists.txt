cmake_minimum_required(VERSION 3.11)
project(Setup)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

target_link_libraries(GraphicsMain PRIVATE glfw)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

# set you main file here
add_executable(GraphicsMain snake_game_3d_main.cpp)

add_subdirectory(helpers)
add_subdirectory(snake_game_3d)
target_link_libraries(GraphicsMain PRIVATE Helpers)

