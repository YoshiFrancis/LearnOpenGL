#include "snake_game_3d.hpp"

#include <random>
#include <string>

Camera *global_camera = nullptr;

std::string get_vec3_str(glm::vec3 &vec) {
  return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " +
         std::to_string(vec.z) + ")";
}

SnakeGame::SnakeGame(GLFWwindow *_window, std::string_view body_fp,
                     std::string_view head_fp, std::string_view tail_fp,
                     std::string_view apple_fp, unsigned int world_height,
                     unsigned int world_width, unsigned int world_length)
    : window(_window), body({std::string(body_fp)}),
      head({std::string(head_fp)}), tail({std::string(tail_fp)}),
      apple({std::string(apple_fp)}), height(world_height), width(world_width),
      length(world_length), shaders("snake_game_3d/shaders/shader.vs",
                                    "snake_game_3d/shaders/shader.fs"),
      camera(glm::vec3(5.0f, 5.0f, world_length + 2)),
      snake_body_pos({glm::vec3(width / 2, height / 2, length / 2)}),
      snake_body_dir({FORWARD}) {
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
      if (try_snake_eat_apple()) {
        total_snake_length += 3;
        ++apples_eaten;
        gen_apple_pos();
      }
      //
      if (try_handle_collision()) {
        std::cout << "collision!\n";
        break;
      }
    }
    display();
  }
  end();
}

void SnakeGame::handle_movement() {
  glm::vec3 new_head_pos;
  print_player_movement_dir();
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
    new_head_pos = glm::vec3(0.f, 0.0f, -1.f);
    break;
  case RIGHT:
    new_head_pos = glm::vec3(1.f, 0.0f, 0.f);
    break;
  case LEFT:
    new_head_pos = glm::vec3(-1.f, 0.0f, 0.f);
    break;
  }

  // add it to the previous head position
  new_head_pos += snake_body_pos.front();
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
  // std::cout << "new head pos: " << get_vec3_str(new_head_pos) << "\n";
  snake_body_pos.push_front(new_head_pos);
  snake_body_dir.push_front(player_movement_dir);

  if (current_snake_length < total_snake_length)
    ++current_snake_length;

  if (snake_body_pos.size() > current_snake_length) {
    snake_body_pos.pop_back();
    snake_body_dir.pop_back();
  }

  // move camera
  Camera_Movement c_movement;
  if (player_movement_dir == UP)
    c_movement = Camera_Movement::UP;
  if (player_movement_dir == DOWN)
    c_movement = Camera_Movement::DOWN;
  if (player_movement_dir == FORWARD)
    c_movement = Camera_Movement::FORWARD;
  if (player_movement_dir == BACKWARD)
    c_movement = Camera_Movement::BACKWARD;
  if (player_movement_dir == RIGHT)
    c_movement = Camera_Movement::RIGHT;
  if (player_movement_dir == LEFT)
    c_movement = Camera_Movement::LEFT;

  // camera.ProcessKeyboard(c_movement, deltaTime);
}

void SnakeGame::end() { return; }

// see if head collides with apple and return true if so
bool SnakeGame::try_snake_eat_apple() { return snake_body_pos[0] == apple_pos; }

// see if head collides with a body part and return true if so
bool SnakeGame::try_handle_collision() {
  for (size_t i = 1; i < snake_body_pos.size(); ++i) {
    if (snake_body_pos[0] == snake_body_pos[i])
      return true;
  }
  return false;
}

// get input from user and handle snake velocity/positions accordingly
bool w_held_down = false;
bool s_held_down = false;
bool d_held_down = false;
bool a_held_down = false;
void SnakeGame::handle_input() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !s_held_down) {
    s_held_down = true;
    if (player_movement_dir == UP)
      player_movement_dir = BACKWARD;
    else if (player_movement_dir == BACKWARD)
      player_movement_dir = DOWN;
    else if (player_movement_dir == DOWN)
      player_movement_dir = FORWARD;
    else if (player_movement_dir == FORWARD)
      player_movement_dir = UP;
    else
      player_movement_dir = UP;
  } else if (glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
    s_held_down = false;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !w_held_down) {
    w_held_down = true;
    if (player_movement_dir == UP)
      player_movement_dir = FORWARD;
    else if (player_movement_dir == BACKWARD)
      player_movement_dir = UP;
    else if (player_movement_dir == DOWN)
      player_movement_dir = BACKWARD;
    else if (player_movement_dir == FORWARD)
      player_movement_dir = DOWN;
    else
      player_movement_dir = DOWN;
  } else if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS)
    w_held_down = false;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !d_held_down) {
    d_held_down = true;
    if (player_movement_dir == FORWARD)
      player_movement_dir = LEFT;
    else if (player_movement_dir == LEFT)
      player_movement_dir = BACKWARD;
    else if (player_movement_dir == BACKWARD)
      player_movement_dir = RIGHT;
    else if (player_movement_dir == RIGHT)
      player_movement_dir = FORWARD;
    else
      player_movement_dir = LEFT;
  } else if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS)
    d_held_down = false;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !a_held_down) {
    std::cout << "d key pressed\n";
    a_held_down = true;
    if (player_movement_dir == FORWARD)
      player_movement_dir = RIGHT;
    else if (player_movement_dir == LEFT)
      player_movement_dir = FORWARD;
    else if (player_movement_dir == BACKWARD)
      player_movement_dir = LEFT;
    else if (player_movement_dir == RIGHT)
      player_movement_dir = BACKWARD;
    else
      player_movement_dir = RIGHT;
  } else if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS)
    a_held_down = false;
}
void SnakeGame::display() {
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // local coords -> world coords -> camera view -> projection
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
  } else if (current_snake_length > 1) {
    // draw last as a body
    model = glm::translate(glm::mat4(1.0f),
                           snake_body_pos[current_snake_length - 1]);
    shaders.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT,
                   (void *)(6 * sizeof(unsigned int)));
  }

  apple.activate();
  shaders.use();
  model = glm::translate(glm::mat4(1.0f), apple_pos);
  shaders.setMat4("model", model);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

std::random_device rd;
std::mt19937 gen(rd());
void SnakeGame::gen_apple_pos() {
  // try randomly getting apple position 10 times!
  for (size_t tries = 0; tries < 10; ++tries) {
    std::uniform_int_distribution<int> rand_width(0, width - 1);
    std::uniform_int_distribution<int> rand_height(0, height - 1);
    std::uniform_int_distribution<int> rand_length(0, length - 1);
    apple_pos = glm::vec3(rand_width(gen), rand_height(gen), rand_length(gen));
    if (check_collision(apple_pos))
      return;
  }

  // brute force!
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < height; ++j) {
      for (size_t k = 0; k < length; ++k) {
        apple_pos = glm::vec3(i, j, k);
        if (check_collision(apple_pos))
          return;
      }
    }
  }
}

const glm::vec3 &SnakeGame::get_apple_pos() const { return apple_pos; }
const std::deque<glm::vec3> &SnakeGame::get_snake_pos() const {
  return snake_body_pos;
}

void SnakeGame::print_player_movement_dir() const {
  switch (player_movement_dir) {
  case UP:
    std::cout << "up\n";
    break;
  case DOWN:
    std::cout << "down\n";
    break;
  case FORWARD:
    std::cout << "forward\n";
    break;
  case BACKWARD:
    std::cout << "backward\n";
    break;
  case RIGHT:
    std::cout << "right\n";
    break;
  case LEFT:
    std::cout << "left\n";
    break;
  }
}

bool SnakeGame::check_collision(glm::vec3 pos, bool include_head) const {
  for (size_t i = 1; i < current_snake_length; ++i) {
    if (pos == snake_body_pos[i])
      return true;
  }

  if (include_head && pos == snake_body_pos[0])
    return true;

  return false;
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
