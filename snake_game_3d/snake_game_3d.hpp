#ifndef SNAKE_GAME_3D_HPP
#define SNAKE_GAME_3D_HPP

#include "shader.hpp"

#include "camera.hpp"
#include "texture.hpp"

#include <GLFW/glfw3.h>

class SnakeGame {
private:
  enum GAME_STATE { MAIN_MENU, GAME, END };
  enum BODY_DIR { UP, DOWN, FORWARD, SIDEWAYS };

  GLFWwindow *window;
  unsigned int VBO, VAO, EBO;
  Texture body, head, tail;
  Shader shaders;
  Camera camera;
  float deltaTime = 0.0f; // Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame
  unsigned int height, width;
  unsigned int apples_eaten = 0;
  unsigned int total_snake_length = 3;
  unsigned int current_snake_length =
      1; // so when eat apple, new length continues to spawn as
         // current_snae_length < total_snake_length
  std::vector<glm::vec3> snake_body_pos;
  std::vector<SnakeGame::BODY_DIR> snake_body_dir;
  glm::vec3 apple_pos;

public:
  SnakeGame(GLFWwindow *window, std::string_view body_fp,
            std::string_view head_fp, std::string_view tail_fp,
            unsigned int world_height = 10, unsigned int world_width = 10);
  ~SnakeGame();
  // main menu
  void begin();
  // actual game
  void loop();
  // show score and go back to main menu
  void end();

private:
  bool try_snake_eat_apple();
  bool try_handle_collision();
  void handle_input();
  void display();
  void gen_apple_pos();
  const glm::vec3 &get_apple_pos() const;
  const std::vector<glm::vec3> &get_snake_pos() const;
};

#endif
