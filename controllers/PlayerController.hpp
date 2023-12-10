#pragma once

#include "GameState.hpp"
#include <array>
#include <atomic>
#include <memory>

class PlayerController {
  public:
    PlayerController() {}

    virtual void publish_state(GameState& state) = 0;

    inline Directions get_direction() {
        return direction;
    };

    inline void set_direction(Directions direction) {
        this->direction = direction;
    };

  private:
    std::atomic<Directions> direction;
};

using PlayerControllerPtr = std::shared_ptr<PlayerController>;
