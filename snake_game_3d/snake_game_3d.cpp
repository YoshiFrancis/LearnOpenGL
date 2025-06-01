#include "snake_game_3d.hpp"


SnakeGame::SnakeGame(std::string_view body_fp, std::string_view head_fp,
        std::string_view tail_fp, unsigned int world_height,
        unsigned int world_width)
    : body({std::string(body_fp)}), head({std::string(head_fp)}), tail({std::string(tail_fp)}), height(world_height), width(world_width)
{

}

SnakeGame::~SnakeGame() 
{

}

void SnakeGame::begin() 
{

}

void SnakeGame::loop()
{

}

void SnakeGame::end()
{

}

void SnakeGame::try_snake_eat_apple() 
{

}
void SnakeGame::try_handle_collision()
{

}
void SnakeGame::handle_input()
{

}
void SnakeGame::display()
{

}
void SnakeGame::gen_apple_pos()
{

}
const glm::vec3 &SnakeGame::get_apple_pos() const
{
    return apple_pos;
}
const std::vector<glm::vec3> &SnakeGame::get_snake_pos() const
{
    return snake_body_pos;
}
