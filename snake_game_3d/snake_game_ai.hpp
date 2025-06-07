#ifndef SNAKE_GAME_AI_HPP
#define SNAKE_GAME_AI_HPP

#include "enums.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>
#include <deque>
#include <optional>
#include <stack>

typedef std::tuple<int, int, int> Coord;

struct Node {
    int nth_node;
    float length;
    Coord pos;
    BODY_DIR curr_dir;
    Node* prev;
    BODY_DIR move_dir;

    Node(int nth, int length_, Coord pos_, BODY_DIR b=BODY_DIR::NONE, Node* p = nullptr) 
        : nth_node(nth), length(length_), pos(pos_), curr_dir(b), prev(p), move_dir(b) {}

    bool operator<(const Node& other) const {
        return length < other.length;
    }

    bool operator==(const Node& other) const {
        return pos == other.pos;
    }
};

struct CompareNodePtr {
    bool operator()(const Node* l, const Node* r) const {
        return l->length > r->length;  
    }
};

/*
 * Algorithm to play my 3D snake game. Utilizes A*. Will account for body parts
 * while going to snake, but will not account for trapping itself in weird
 * positions right after apple!
 */

class SnakeGameAI {
private:
  const std::deque<glm::vec3> &snake_body_pos;
  const glm::vec3 &apple_pos;
  std::stack<BODY_DIR> moveset;
  int width, height, length;
  BODY_DIR curr_head_dir = BODY_DIR::FORWARD;

public:
  SnakeGameAI(const std::deque<glm::vec3> &snake_body_pos_,
              const glm::vec3 &apple_pos_, int width_, int height_, int length_);

  BODY_DIR get_next_move(BODY_DIR head_pos);
  void force_update();

private:
  void generate_moveset();
  // 5 nodes to generate: 4 on side, 1 in direction already headed
  std::vector<Node*> generate_adjacent_nodes(Node& n) const;
  bool try_check_collision(Coord coord) const;
  int manhattan_distance(int x, int y, int z) const;
};

#endif
