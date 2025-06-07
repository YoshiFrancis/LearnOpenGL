#include "snake_game_ai.hpp"

#include <set>

#include <iostream>
#include <queue>

SnakeGameAI::SnakeGameAI(const std::deque<glm::vec3> &snake_body_pos_,
                         const glm::vec3 &apple_pos_, int width_, int height_,
                         int length_)
    : snake_body_pos(snake_body_pos_), apple_pos(apple_pos_), width(width_),
      height(height_), length(length_) {}

std::string coord_str(Coord coord) {
  return "(" + std::to_string(std::get<0>(coord)) + ", " +
         std::to_string(std::get<1>(coord)) + ", " +
         std::to_string(std::get<2>(coord)) + ")";
}

void print_path(Node *last) {
  while (last->prev != nullptr) {
    std::string dir;
    switch (last->curr_dir) {
    case BODY_DIR::UP:
      dir = "up\n";
      break;
    case BODY_DIR::DOWN:
      dir = "down\n";
      break;
    case BODY_DIR::FORWARD:
      dir = "forward\n";
      break;
    case BODY_DIR::BACKWARD:
      dir = "backward\n";
      break;
    case BODY_DIR::RIGHT:
      dir = "right\n";
      break;
    case BODY_DIR::LEFT:
      dir = "left\n";
      break;
    default:
      dir = "No dir\n";
    }
    std::cout << coord_str(last->pos) << " | length: " << last->length
              << ", direction : " << dir;
    last = last->prev;
  }
}

BODY_DIR SnakeGameAI::get_next_move(BODY_DIR head_pos) {
  if (moveset.size() == 0) {
    curr_head_dir = head_pos;
    generate_moveset();
  }
  if (moveset.empty()) {
    return BODY_DIR::NONE;
  } else {
    BODY_DIR next_move = moveset.top();
    moveset.pop();
    return next_move;
  }
}

void SnakeGameAI::force_update() { generate_moveset(); }

void SnakeGameAI::generate_moveset() {
  // clear the moveset
  while (moveset.size() > 0) {
    moveset.pop();
  }

  // run A* algorithm

  Node target_node = Node(0, 0, {apple_pos.x, apple_pos.y, apple_pos.z});
  std::priority_queue<Node *, std::vector<Node *>, CompareNodePtr> q;
  std::set<Coord> seen;
  Node *first_node = new Node(
      0, 0, {snake_body_pos[0].x, snake_body_pos[0].y, snake_body_pos[0].z},
      curr_head_dir, nullptr);
  q.push(first_node);
  Node *curr_node;
  while (!q.empty()) {
    curr_node = q.top();
    q.pop();
    // std::cout << curr_node->length << "\n";
    if (*curr_node == target_node)
      break;
    else if (seen.count(curr_node->pos) == 0) {
      seen.insert(curr_node->pos);
      if (try_check_collision(curr_node->pos))
        continue;
      auto nodes = generate_adjacent_nodes(*curr_node);
      for (Node *node : nodes) {
        q.push(node);
      }
    }
  }

  std::cout << "done\n";
  if (*curr_node == target_node) {
    print_path(curr_node);
    while (curr_node->prev != nullptr) {
      moveset.push(curr_node->move_dir);
      curr_node = curr_node->prev;
    }
  }
}

std::vector<Node *> SnakeGameAI::generate_adjacent_nodes(Node &n) const {
  std::vector<Node *> nodes;
  // check forward direction -- cannot allow counterpart to do this direction as
  // you cannot do full 180
  if (n.curr_dir != BODY_DIR::BACKWARD) {
    auto [x, y, z] = n.pos;
    ++z;
    if (z >= length)
      z = 0;
    float weight = manhattan_distance(x, y, z);
    if (weight != INT_MAX)
      weight += n.nth_node;
    Node *forward_node =
        new Node(n.nth_node + 1, weight, {x, y, z}, BODY_DIR::FORWARD, &n);
    nodes.emplace_back(forward_node);
  }

  if (n.curr_dir != BODY_DIR::FORWARD) {
    auto [x, y, z] = n.pos;
    --z;
    if (z < 0)
      z = length - 1;
    float weight = manhattan_distance(x, y, z);
    if (weight != INT_MAX)
      weight += n.nth_node;
    Node *backward_node =
        new Node(n.nth_node + 1, weight, {x, y, z}, BODY_DIR::BACKWARD, &n);
    nodes.emplace_back(backward_node);
  }

  if (n.curr_dir != BODY_DIR::DOWN) {
    auto [x, y, z] = n.pos;
    ++y;
    if (y >= height)
      y = 0;
    float weight = manhattan_distance(x, y, z);
    if (weight != INT_MAX)
      weight += n.nth_node;
    Node *up_node =
        new Node(n.nth_node + 1, weight, {x, y, z}, BODY_DIR::UP, &n);
    nodes.emplace_back(up_node);
  }

  if (n.curr_dir != BODY_DIR::UP) {
    auto [x, y, z] = n.pos;
    --y;
    if (y < 0)
      y = height - 1;
    float weight = manhattan_distance(x, y, z);
    if (weight != INT_MAX)
      weight += n.nth_node;
    Node *down_node =
        new Node(n.nth_node + 1, weight, {x, y, z}, BODY_DIR::DOWN, &n);
    nodes.emplace_back(down_node);
  }

  if (n.curr_dir != BODY_DIR::LEFT) {
    auto [x, y, z] = n.pos;
    ++x;
    if (x >= width)
      x = 0;
    float weight = manhattan_distance(x, y, z);
    if (weight != INT_MAX)
      weight += n.nth_node;
    Node *right_node =
        new Node(n.nth_node + 1, weight, {x, y, z}, BODY_DIR::RIGHT, &n);
    nodes.emplace_back(right_node);
  }

  if (n.curr_dir != BODY_DIR::RIGHT) {
    auto [x, y, z] = n.pos;
    --x;
    if (x < 0)
      x = width - 1;
    float weight = manhattan_distance(x, y, z);
    if (weight != INT_MAX)
      weight += n.nth_node;
    Node *left_node =
        new Node(n.nth_node + 1, weight, {x, y, z}, BODY_DIR::LEFT, &n);
    nodes.emplace_back(left_node);
  }

  for (auto *node : nodes) {
    if (node->curr_dir == n.curr_dir)
      node->move_dir = BODY_DIR::NONE;
    // std::cout << "node length: " << node->length << '\n';
  }
  return nodes;
}

bool SnakeGameAI::try_check_collision(Coord coord) const {
  // dont check head! head is always the first node checked
  auto [x, y, z] = coord;
  for (size_t i = 1; i < snake_body_pos.size(); ++i) {
    if (x == snake_body_pos[i].x && y == snake_body_pos[i].y &&
        z == snake_body_pos[i].z)
      return true;
  }
  return false;
}

int SnakeGameAI::manhattan_distance(int x, int y, int z) const {
  if (try_check_collision({x, y, z}))
    return INT_MAX;
  return abs(x - apple_pos.x) + abs(y - apple_pos.y) + abs(z - apple_pos.z);
}
