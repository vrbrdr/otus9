#include "Player.hpp"
#include <iostream>
#include <stdexcept>

SnakeSegment Player::make_head(SnakeSegment& prev_head) {
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

void Player::Move(uint64_t move_segments) {
    if (snake->IsDie()) {
        return;
    }

    for (int i = 0; i < move_segments; i++) {
        snake->tile_direction =
            snake->TileDirection(snake->body[snake->body.size() - 2],
                                 snake->body[snake->body.size() - 1]);

        snake->body.pop_back();
        auto head = snake->body.begin();
        auto new_head = make_head(*head);
        snake->body.insert(head, new_head);
    }

    snake->direction = controller->get_direction();
}