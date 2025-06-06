#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "snake_game_3d.hpp"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr int SNAKE_GAME_LENGTH = 10;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to initialize GLAD\n";
    return -1;
  }
  SnakeGame game(window, "snake_game_3d/textures/body.jpg",
                 "snake_game_3d/textures/head.jpg",
                 "snake_game_3d/textures/tail.jpg", "snake_game_3d/textures/tail.jpg");
  game.begin();
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
