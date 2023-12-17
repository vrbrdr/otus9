#pragma once

#include "controllers\PlayerController.hpp"
#include <memory>
#include <string>

class Player {
  private:
    uint8_t index;
    std::string name;
    PlayerControllerPtr controller;

  public:
    const SnakePtr snake;

    Player(uint8_t index, PlayerControllerPtr controller)
        : index{index}, name{std::string{"Player "} + std::to_string(index)},
          controller{controller}, snake{std::make_shared<Snake>(index)} {

        index = controller->Connect(index);
    }

    void Move(uint64_t move_segments);

    inline void Publish(GameState& state) {
        controller->Publish(index, state);
    }

    inline void SetDirection(Directions direction) {
        snake->direction = direction;
        controller->set_direction(direction);
    }

    inline bool IsDie() {
        return snake->IsDie();
    }

  private:
    SnakeSegment make_head( SnakeSegment& segment);
};

using PlayerPtr = std::shared_ptr<Player>;
using Players = std::vector<PlayerPtr>;