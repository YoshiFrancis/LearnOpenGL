#ifndef SNAKE_GAME_3D_HPP
#define SNAKE_GAME_3D_HPP

#include "../texture.hpp"
#include "QuadTree.hpp"

class SnakeGame {
private:
  enum GAME_STATE { MAIN_MENU, GAME, END };
  enum BODY_DIR { UP, DOWN, FORWARD, SIDEWAYS };

  unsigned int VBO, VAO, EBO;
  Texture body, head, tail;
  QuadTree world;
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
  SnakeGame(std::string_view body_fp, std::string_view head_fp,
            std::string_view tail_fp, unsigned int world_height = 10,
            unsigned int world_width = 10);
  ~SnakeGame();
  // main menu
  void begin();
  // actual game
  void loop();
  // show score and go back to main menu
  void end();

private:
  void try_snake_eat_apple();
  void try_handle_collision();
  void handle_input();
  void display();
  void gen_apple_pos();
  const QuadTree &get_world() const;
  glm::vec3 &get_apple_pos() const;
  std::vector<glm::vec3> &get_snake_pos() const;
};

#endif
