#pragma once

#include "PlayerController.hpp"

class KeyboardPlayerController : public PlayerController {
  private:
    std::array<Directions, 4> reverse;

  public:
    KeyboardPlayerController() {
        reverse[(int)Directions::UP] = Directions::DOWN;
        reverse[(int)Directions::DOWN] = Directions::UP;
        reverse[(int)Directions::LEFT] = Directions::RIGTH;
        reverse[(int)Directions::RIGTH] = Directions::LEFT;
    }
    virtual void publish_state(GameState& state) override{};

    inline void KeyPressed(Directions direction) {
        // Блокируем разворот на 180
        if (reverse[(int)(direction)] == get_direction()) {
            return;
        }

        set_direction(direction);
    }
};
