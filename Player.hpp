#pragma once

#include "controllers\PlayerController.hpp"
#include <memory>
#include <string>

class Player {
  private:
    std::string name;
    PlayerControllerPtr controller;

  public:
    SnakePtr snake;

    Player(uint8_t player, PlayerControllerPtr controller)
        : controller{controller} {
        auto real_player = controller->Connect(player);
        name = std::string{"Player "} + std::to_string(real_player);
        snake = std::make_shared<Snake>(real_player);
    }

    void Move(uint64_t move_segments);


    inline void Publish(GameState& state) {
        controller->Exchange(state);
    }

    inline void SetDirection(Directions direction) {
        snake->direction = direction;
        controller->set_direction(direction);
    }

    inline bool IsDie() {
        return snake->IsDie();
    }

    inline PlayerControllerPtr getControler() {
        return controller;
    }

  private:
    SnakeSegment make_head(SnakeSegment& segment);
};

using PlayerPtr = std::shared_ptr<Player>;
using Players = std::vector<PlayerPtr>;