#pragma once

#include "../network/Message.hpp"
#include "../network/Network.hpp"
#include "PlayerController.hpp"

class RemotePlayerController : public PlayerController {
  private:
    const std::unique_ptr<NetExchange> connection;

  public:
    RemotePlayerController(std::unique_ptr<NetExchange> connection)
        : connection{connection.release()} {}

    ~RemotePlayerController() override {
        PlayerController::~PlayerController();
    }

    uint8_t Connect(uint8_t index) override {
        return index;
    }

    virtual void Publish(uint8_t player, GameState& state) override {
        connection->Send(Message{player, state});
    };

    virtual Directions get_direction() override {
        //!!!!!! подумать об использовании total_distance;
        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            PlayerController::set_direction(message.GetUserState().direction);
        }

        return PlayerController::get_direction();
    }
};
