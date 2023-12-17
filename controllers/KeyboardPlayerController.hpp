#pragma once

#include "PlayerController.hpp"

class KeyboardPlayerController : public PlayerController {
  public:
    KeyboardPlayerController() {}

    ~KeyboardPlayerController() override {
        PlayerController::~PlayerController();
    };

    uint8_t Connect(uint8_t index) override {
        return index;
    }

#pragma warning(push)
#pragma warning(disable : 4100)
    virtual void Exchange(GameState& state) override{};
#pragma warning(pop)

    Directions get_direction() override {
        return PlayerController::get_direction();
    }

    void set_direction(Directions _direction) override {
        return PlayerController::set_direction(_direction);
    }
};
