#pragma once

#include "../GameState.hpp"
#include <array>
#include <atomic>
#include <memory>

class PlayerController {
  public:
    PlayerController() {}
    virtual ~PlayerController() {};

    virtual uint8_t Connect(uint8_t index) = 0;
    
    virtual void Publish(uint8_t player, GameState& state) = 0;

    virtual Directions get_direction() {
        return direction;
    };

    inline void set_direction(Directions _direction) {
        direction = _direction;
    };

  private:
    std::atomic<Directions> direction;
};

using PlayerControllerPtr = std::shared_ptr<PlayerController>;
