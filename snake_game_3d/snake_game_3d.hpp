#ifndef SNAKE_GAME_3D_HPP
#define SNAKE_GAME_3D_HPP

#include "shader.hpp"

#include "camera.hpp"
#include "texture.hpp"

#include <deque>

#include <GLFW/glfw3.h>

constexpr float fov = 45.0f;
constexpr float frame_time = 0.5f;
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

class SnakeGame {
private:
  enum GAME_STATE { MAIN_MENU, GAME, END };
  enum BODY_DIR { UP, DOWN, FORWARD, BACKWARD, RIGHT, LEFT };

  GLFWwindow *window;
  unsigned int VBO, VAO, EBO;
  Texture body, head, tail, apple;
  Shader shaders;
  Camera camera;
  float deltaTime = 0.0f; // Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame
  float time_between_frame = 0.0f;
  unsigned int width, height, length; // x, y, z boundaries respectively
  BODY_DIR player_movement_dir = FORWARD;
  unsigned int apples_eaten = 0;
  unsigned int total_snake_length = 5;
  unsigned int current_snake_length =
      1; // so when eat apple, new length continues to spawn as
         // current_snae_length < total_snake_length
  std::deque<glm::vec3> snake_body_pos;
  std::deque<SnakeGame::BODY_DIR> snake_body_dir;
  glm::vec3 apple_pos;

public:
  SnakeGame(GLFWwindow *window, std::string_view body_fp,
            std::string_view head_fp, std::string_view tail_fp, std::string_view apple_fp,
            unsigned int world_height = 10, unsigned int world_width = 10,
            unsigned int world_length = 10);
  ~SnakeGame();
  // main menu
  void begin();
  // show score and go back to main menu
  void end();

private:
  // actual game
  void loop();

  // loop functions
  void handle_input();
  void handle_movement();
  bool try_snake_eat_apple();
  bool try_handle_collision();
  void display();

  // helper functions
  void gen_apple_pos();
  const glm::vec3 &get_apple_pos() const;
  const std::deque<glm::vec3> &get_snake_pos() const;
  void print_player_movement_dir() const;
  bool check_collision(glm::vec3 pos, bool include_head=true) const;
};

#endif
