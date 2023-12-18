#pragma once

#include "PlayerController.hpp"

class EmptyController : public PlayerController {
  private:
    SnakePtr snake;

  public:
    EmptyController(SnakePtr snake) : snake{snake} {};

    ~EmptyController() override {
        PlayerController::~PlayerController();
    }

    std::shared_ptr<GameState> GetState() override {
        return nullptr;
    }

    virtual void SendState(GameState&) override {}

    uint8_t Connect(uint8_t index) override {
        return index;
    }

    Directions get_direction() override {
        return snake->get_direction();
    }
};
