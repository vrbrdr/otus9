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
    uint8_t index;

    Player(uint8_t _player, PlayerControllerPtr controller)
        :index{_player}, controller{controller} {
        name = std::string{"Player "} + std::to_string(index);
        snake = std::make_shared<Snake>(index);
    }

    void Move(uint64_t move_segments);

    inline void Exchange(GameState& state) {
        controller->SendState(state);
        auto new_state = controller->GetState();
        if(new_state){
            state.Update(*new_state);
        }
    }

    inline void SetDirection(Directions direction) {
        snake->set_direction(direction);
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