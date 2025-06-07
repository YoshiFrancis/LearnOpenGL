#include "snake_game_ai.hpp"

#include <set>

#include <iostream>
#include <queue>

SnakeGameAI::SnakeGameAI(const std::deque<glm::vec3> &snake_body_pos_,
        const glm::vec3 &apple_pos_, int width_, int height_, int length_)
    : snake_body_pos(snake_body_pos_), apple_pos(apple_pos_), width(width_), height(height_), length(length_)
{ }

void print_path(Node* last) {
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
    while (last) {
        std::cout << "(" << last->x << ", " << last->y << ", " << last->z << "), length: " << last->length << ", direction: " << dir;
        last = last->prev;
    }
    
}

BODY_DIR SnakeGameAI::get_next_move() {
    if(moveset.size() == 0) 
        generate_moveset();

    BODY_DIR next_move = moveset.top();
    moveset.pop();
    return next_move;
}

void SnakeGameAI::force_update() {
    generate_moveset();
}

void SnakeGameAI::generate_moveset() {
    // clear the moveset
    while (moveset.size() > 0)
    {
        moveset.pop();
    }

    // run A* algorithm

    Node target_node = Node(0, apple_pos.x, apple_pos.y, apple_pos.z);
    std::priority_queue<Node*, std::vector<Node*>, CompareNodePtr> q;
    std::set<Node> seen;
    Node* first_node = new Node(0, snake_body_pos[0].x, snake_body_pos[0].y, snake_body_pos[0].z, curr_head_dir, nullptr);
    q.push(first_node);
    Node* curr_node;
    while (!q.empty()) {
        curr_node = q.top();
        q.pop();
        if (*curr_node == target_node) 
            break;
        else if (seen.count(*curr_node) == 0) {
            seen.insert(*curr_node);
            if (try_check_collision(*curr_node)) 
                continue;
            auto nodes = generate_adjacent_nodes(*curr_node);
            for (Node* node : nodes) {
                q.push(node);
            }
        }
    }

    if (*curr_node == target_node) {
        print_path(curr_node);
        while (curr_node) {
            moveset.push(curr_node->curr_dir);
            curr_node = curr_node->prev;
        }
        std::cout << "success!\n";
    }
}


std::vector<Node*> SnakeGameAI::generate_adjacent_nodes(Node& n) const {
    std::vector<Node*> nodes;

    // check forward direction -- cannot allow counterpart to do this direction as you cannot do full 180
    if (n.curr_dir != BODY_DIR::BACKWARD) {
        Node* forward_node = new Node(0, n.x, n.y, n.z + 1, BODY_DIR::FORWARD, &n);
        if (forward_node->z >= length) forward_node->z = 0; // wrap around
        float heuristic = manhattan_distance(n.x, n.y, forward_node->z) + n.length + 1;
        forward_node->length = heuristic;
        nodes.emplace_back(forward_node);
    }

    if (n.curr_dir != BODY_DIR::FORWARD) {
        Node* backward_node = new Node(0, n.x, n.y, n.z - 1, BODY_DIR::BACKWARD, &n);
        if (backward_node->z < 0) backward_node->z = length - 1; // wrap around
        float heuristic = manhattan_distance(n.x, n.y, backward_node->z) + n.length + 1;
        backward_node->length = heuristic;
        nodes.emplace_back(backward_node);
    }

    if (n.curr_dir != BODY_DIR::DOWN) {
        Node* up_node = new Node(0, n.x, n.y+1, n.z, BODY_DIR::UP, &n);
        if (up_node->y >= height) up_node->y = 0;
        float heuristic = manhattan_distance(n.x, up_node->y, n.z) + n.length + 1;
        up_node->length = heuristic;
        nodes.emplace_back(up_node);
    }

    if (n.curr_dir != BODY_DIR::UP) {
        Node* down_node = new Node(0, n.x, n.y-1, n.z, BODY_DIR::DOWN, &n);
        if (down_node->y < 0) down_node->y = height - 1;
        float heuristic = manhattan_distance(n.x, down_node->y, n.z) + n.length + 1;
        down_node->length = heuristic;
        nodes.emplace_back(down_node);
    }
    
    if (n.curr_dir != BODY_DIR::LEFT) {
        Node* right_node = new Node(0, n.x+1, n.y, n.z, BODY_DIR::RIGHT, &n);
        if (right_node->x >= width) right_node->x = 0;
        float heuristic = manhattan_distance(right_node->x, n.y, n.z) + n.length + 1;
        right_node->length = heuristic;
        nodes.emplace_back(right_node);
    }

    if (n.curr_dir != BODY_DIR::RIGHT) {
        Node* left_node = new Node(0, n.x-1, n.y, n.z, BODY_DIR::LEFT, &n);
        if (left_node->x < 0) left_node->x = width - 1;
        float heuristic = manhattan_distance(left_node->x, n.y, n.z) + n.length + 1;
        left_node->length = heuristic;
        nodes.emplace_back(left_node);
    }
    return nodes;
}

bool SnakeGameAI::try_check_collision(const Node& n) const {
    // dont check head! head is always the first node checked
    for (size_t i = 1; i < snake_body_pos.size(); ++i) {
        if (n.x == snake_body_pos[i].x && n.y == snake_body_pos[i].y && n.z == snake_body_pos[i].z) return true;
    }
    return false;
}

float SnakeGameAI::manhattan_distance(int x, int y, int z) const {
    return abs(x - apple_pos.x) + abs(y - apple_pos.y) + abs(z - apple_pos.z);
}
