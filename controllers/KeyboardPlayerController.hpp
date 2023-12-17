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

    ~KeyboardPlayerController() override {
        PlayerController::~PlayerController();
    };

    uint8_t Connect(uint8_t index) override {
        return index;
    }

#pragma warning(push)
#pragma warning(disable : 4100)
    virtual void Publish(uint8_t player, GameState& state) override{};
#pragma warning(pop)

    inline void KeyPressed(Directions _direction) {
        // Блокируем разворот на 180
        if (reverse[(int)(_direction)] == get_direction()) {
            return;
        }

        set_direction(_direction);
    }
};
