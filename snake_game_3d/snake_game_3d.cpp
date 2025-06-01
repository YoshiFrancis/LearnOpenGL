#include "snake_game_3d.hpp"

#include <string>

SnakeGame::SnakeGame(GLFWwindow *_window, std::string_view body_fp,
                     std::string_view head_fp, std::string_view tail_fp,
                     unsigned int world_height, unsigned int world_width)
    : window(_window), body({std::string(body_fp)}),
      head({std::string(head_fp)}), tail({std::string(tail_fp)}),
      height(world_height), width(world_width),
      shaders("shader.vs", "shader.fs"), camera(glm::vec3(0.0f, 0.0f, 3.0f)) {

  float vertices[] = {
      // positions         // texcoords
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // 1
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // 2
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // 3
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // 4
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, // 5
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f  // 7
  };

  unsigned int cube_indicies[] = {
      4, 5, 6, 6, 7, 4, // front
      4, 7, 3, 3, 0, 4, // left
      1, 5, 6, 6, 2, 1, // right
      3, 2, 6, 6, 7, 3, // top
      4, 0, 1, 1, 5, 4, // bottom
      0, 1, 2, 2, 3, 0, // back
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // for first triangle
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indicies), cube_indicies,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glEnable(GL_DEPTH_TEST);
}

SnakeGame::~SnakeGame() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void SnakeGame::begin() { loop(); }

void SnakeGame::loop() {
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // gets input from user and moves user acoordingly
    handle_input();

    if (try_snake_eat_apple()) {
    }

    if (try_handle_collision()) {
      // TODO
    }

    display();
  }
  end();
}

void SnakeGame::end() { return; }

// see if head collides with apple and return true if so
bool SnakeGame::try_snake_eat_apple() { return false; }

// see if head collides with a body part and return true if so
bool SnakeGame::try_handle_collision() { return false; }

// get input from user and handle snake velocity/positions accordingly
// TODO
void SnakeGame::handle_input() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}
void SnakeGame::display() {
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw head and setup camera
  // TODO
  head.activate();
  shaders.use();

  // draw body
  // TODO
  body.activate();
  shaders.use();

  // draw tail
  // TODO
  tail.activate();
  shaders.use();
}
void SnakeGame::gen_apple_pos() {}
const glm::vec3 &SnakeGame::get_apple_pos() const { return apple_pos; }
const std::vector<glm::vec3> &SnakeGame::get_snake_pos() const {
  return snake_body_pos;
}
