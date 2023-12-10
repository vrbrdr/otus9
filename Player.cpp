#include "Player.hpp"
#include <iostream>
#include <stdexcept>

namespace {
#if true
    void print(char* header, SnakePtr snake){};
#else
    void print(char* header, SnakePtr snake) {
        std::cout << "dir: " << (int)(snake->direction) << "; ";

        std::cout << header << ": ";
        for (auto& s : snake->body) {
            std::cout << "[" << s.x << "," << s.y << "], ";
        }
        std::cout << std::endl;
    }
#endif

} // namespace

SnakeSegment Player::make_head(SnakePtr snake, SnakeSegment& prev_head) {
    switch (snake->direction) {
    case Directions::LEFT:
        return SnakeSegment{prev_head.x - 1, prev_head.y};

    case Directions::RIGTH:
        return SnakeSegment{prev_head.x + 1, prev_head.y};

    case Directions::UP:
        return SnakeSegment{prev_head.x, prev_head.y - 1};

    case Directions::DOWN:
        return SnakeSegment{prev_head.x, prev_head.y + 1};
    }

    throw new std::logic_error("Unknown direction");
}

void Player::Move(uint64_t distance) {

    auto total_distance = snake->percent + distance;
    snake->percent = total_distance % 100;
    auto move_segments = total_distance / 100;
    auto head = *(snake->body.begin());

    for (int i = 0; i < move_segments; i++) {
        print("before", snake);
        snake->body.pop_back();
        auto head = snake->body.begin();
        auto new_head = make_head(snake, *head);
        snake->body.insert(head, new_head);
        print("after ", snake);
    }

    snake->direction = controller->get_direction();
}