#pragma once

#include "controllers\PlayerController.hpp"
#include <memory>
#include <string>

class Player {
  private:
    std::string name;
    PlayerControllerPtr controller;

  public:
    const SnakePtr snake;

    Player(PlayerControllerPtr controller)
        : controller{controller}, snake{std::make_shared<Snake>()} {}

    void Move(uint64_t distance);

    inline void Publish(GameState& state) {
        controller->publish_state(state);
    }

    inline void SetDirection(Directions direction) {
        snake->direction = direction;
        controller->set_direction(direction);
    }

  private:
    SnakeSegment make_head(SnakePtr snake, SnakeSegment& segment);
};

using PlayerPtr = std::shared_ptr<Player>;
using Players = std::vector<PlayerPtr>;