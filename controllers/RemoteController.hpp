#pragma once

#include "../network/Message.hpp"
#include "../network/Network.hpp"
#include "PlayerController.hpp"

class RemoteController : public PlayerController {
  protected:
    const std::shared_ptr<NetExchange> connection;
    uint8_t player;

  public:
    RemoteController(std::shared_ptr<NetExchange> connection)
        : connection{connection} {}

    ~RemoteController() override {
        PlayerController::~PlayerController();
    }

    uint8_t Connect(uint8_t index) override {
        player = index;
        return player;
    }
};