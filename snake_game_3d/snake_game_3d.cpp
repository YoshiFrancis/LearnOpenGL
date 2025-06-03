#include "snake_game_3d.hpp"

#include <string>

Camera *global_camera = nullptr;

SnakeGame::SnakeGame(GLFWwindow *_window, std::string_view body_fp,
                     std::string_view head_fp, std::string_view tail_fp,
                     unsigned int world_height, unsigned int world_width,
                     unsigned int world_length)
    : window(_window), body({std::string(body_fp)}),
      head({std::string(head_fp)}), tail({std::string(tail_fp)}),
      height(world_height), width(world_width), length(world_length),
      shaders("snake_game_3d/shaders/shader.vs",
              "snake_game_3d/shaders/shader.fs"),
      camera(glm::vec3(0.0f, 0.0f, 3.0f)),
      snake_body_pos({glm::vec3(1.0f, 1.0f, 1.0f)}), snake_body_dir({FORWARD}) {
  global_camera = &camera;
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    glfwSwapBuffers(window);
    glfwPollEvents();
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    time_between_frame += deltaTime;
    handle_input();
    if (time_between_frame >= frame_time) {
      time_between_frame = 0;
      std::cout << "frame\n";
      // gets input from user and moves user acoordingly
      handle_movement();
      //
      // if (try_snake_eat_apple()) {
      // }
      //
      // if (try_handle_collision()) {
      //   // TODO
      // }
      //
    }
    display();
  }
  end();
}

void SnakeGame::handle_movement() {
  glm::vec3 new_head_pos = glm::vec3(0.0f, 0.0f, 0.0f);
  switch (player_movement_dir) {
  case UP:
    new_head_pos = glm::vec3(0.f, 1.0f, 0.f);
    break;
  case DOWN:
    new_head_pos = glm::vec3(0.f, -1.0f, 0.f);
    break;
  case FORWARD:
    new_head_pos = glm::vec3(0.f, 0.0f, 1.f);
    break;
  case BACKWARD:
    new_head_pos = glm::vec3(0.f, 1.0f, -1.f);
    break;
  case RIGHT:
    new_head_pos = glm::vec3(1.f, 0.0f, 0.f);
    break;
  case LEFT:
    new_head_pos = glm::vec3(-1.f, 0.0f, 0.f);
    break;
  }

  // add it to the previous head position
  new_head_pos += snake_body_pos.back();
  // checking if it goes over bounds
  // no need to check all axises individually due to no diagonol movement
  if (new_head_pos.x < 0)
    new_head_pos.x = width - 1;
  else if (new_head_pos.x >= width)
    new_head_pos.x = 0;
  else if (new_head_pos.y >= height)
    new_head_pos.y = 0;
  else if (new_head_pos.y < 0)
    new_head_pos.y = height - 1;
  else if (new_head_pos.z < 0)
    new_head_pos.z = length - 1;
  else if (new_head_pos.z >= length)
    new_head_pos.z = 0;

  // no movement!
  // REMOVE LATER
  // new_head_pos = glm::vec3(0.f, 0.f, 0.f);
  //
  snake_body_pos.push_front(new_head_pos);
  snake_body_dir.push_front(player_movement_dir);

  if (current_snake_length < total_snake_length)
    ++current_snake_length;

  if (snake_body_pos.size() > current_snake_length) {
    snake_body_pos.pop_back();
    snake_body_dir.pop_back();
  }
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
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
  }
}
void SnakeGame::display() {
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // settup camera
  glm::mat4 projection = glm::perspective(
      glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);

  glm::mat4 view = camera.GetViewMatrix();
  //
  shaders.setMat4("projection", projection);
  shaders.setMat4("view", view);
  //
  // // draw head
  head.activate();
  shaders.use();
  glm::mat4 model = glm::translate(glm::mat4(1.0f), snake_body_pos[0]);
  shaders.setMat4("model", model);
  glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT,
                 (void *)(0 * sizeof(unsigned int)));
  //
  // // draw body
  body.activate();
  shaders.use();
  for (size_t i = 1; i < current_snake_length - 1; ++i) {
    model = glm::translate(glm::mat4(1.0f), snake_body_pos[i]);
    shaders.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT,
                   (void *)(6 * sizeof(unsigned int)));
  }

  // // draw tail
  if (current_snake_length == total_snake_length) {
    tail.activate();
    shaders.use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f),
                                     snake_body_pos[current_snake_length - 1]);
    shaders.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT,
                   (void *)(6 * sizeof(unsigned int)));
  }
}
void SnakeGame::gen_apple_pos() {}
const glm::vec3 &SnakeGame::get_apple_pos() const { return apple_pos; }
const std::deque<glm::vec3> &SnakeGame::get_snake_pos() const {
  return snake_body_pos;
}

bool firstMouse = true;
int lastX = 0;
int lastY = 0;

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  if (global_camera != nullptr)
    global_camera->ProcessMouseMovement(xoffset, yoffset);
}
