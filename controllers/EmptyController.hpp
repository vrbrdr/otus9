
#pragma once

#include "PlayerController.hpp"

class EmptyController : public PlayerController {
  protected:
  public:
    EmptyController() = default;

    ~EmptyController() override {
        PlayerController::~PlayerController();
    }

#pragma warning(push)
#pragma warning(disable : 4100)
        std::shared_ptr<GameState> GetState() override {
        return nullptr;
    }

    virtual void SendState(GameState& state) override {}
#pragma warning(pop)

    uint8_t Connect(uint8_t index) override {
        return index;
    }
};
