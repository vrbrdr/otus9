#pragma once

#include "../network/Network.hpp"
#include "KeyboardPlayerController.hpp"

class RemoteServerController : public KeyboardPlayerController {
  private:
    const std::unique_ptr<NetExchange> connection;

  public:
    RemoteServerController(std::unique_ptr<NetExchange> connection)
        : connection{connection.release()} {}

    ~RemoteServerController() override {
        KeyboardPlayerController::~KeyboardPlayerController();
    }

#pragma warning(push)
#pragma warning(disable : 4100)
    uint8_t Connect(uint8_t index) override {
        for (;;) {
            auto const& message = connection->Receive();
            if (!message.IsEmpty()) {
                return message.player;
            }
        }
    }
#pragma warning(pop)

    void Publish(uint8_t player, GameState& state) override {
        connection->Send(
            Message{player, UserState{state.total_distance, get_direction()}});

        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            state.Update(message.GetGameState());
        }
    }
};
