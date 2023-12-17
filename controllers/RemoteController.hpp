#pragma once

#include "../network/Message.hpp"
#include "../network/Network.hpp"
#include "PlayerController.hpp"

class RemoteController : public PlayerController {
  protected:
    const std::unique_ptr<NetExchange> connection;
    uint8_t player;

  public:
    RemoteController(std::unique_ptr<NetExchange> connection)
        : connection{connection.release()} {}

    ~RemoteController() override {
        PlayerController::~PlayerController();
    }

    uint8_t Connect(uint8_t index) override {
        player = index;
        return player;
    };
};