#pragma once

#include "../GameState.hpp"
#include <array>
#include <atomic>
#include <memory>

class PlayerController {
  public:
    PlayerController() {
        reverse[(int)Directions::UP] = Directions::DOWN;
        reverse[(int)Directions::DOWN] = Directions::UP;
        reverse[(int)Directions::LEFT] = Directions::RIGTH;
        reverse[(int)Directions::RIGTH] = Directions::LEFT;
    }
    
    virtual ~PlayerController() {};

    virtual uint8_t Connect(uint8_t index) {
        return index;
    };

    virtual void Exchange(GameState& state) = 0;

    virtual  Directions get_direction() {
        return direction;
    };

    virtual void set_direction(Directions _direction) {
              // Блокируем разворот на 180
        if (reverse[(int)(_direction)] == get_direction()) {
            return;
        }

        direction = _direction;
    };

  private:
    std::array<Directions, 4> reverse;
    Directions direction = Directions::UNDEFINED;
};

using PlayerControllerPtr = std::shared_ptr<PlayerController>;
