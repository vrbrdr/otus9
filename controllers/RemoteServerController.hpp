#pragma once

#include "../network/Network.hpp"
#include "RemoteController.hpp"

class RemoteServerController : public RemoteController {
  private:
    bool force_publish = true;
    uint64_t last_publish_distance = 0;

  public:
    RemoteServerController(std::unique_ptr<NetExchange> connection)
        : RemoteController(std::move(connection)) {}

    ~RemoteServerController() override {
        RemoteController::~RemoteController();
    }

#pragma warning(push)
#pragma warning(disable : 4100)
    uint8_t Connect(uint8_t index) override {
        for (;;) {
            auto const& message = connection->Receive();
            if (!message.IsEmpty()) {
                player = message.player;
                return RemoteController::Connect(player);
            }
        }
    }
#pragma warning(pop)

    void Exchange(GameState& state) override {
        if (force_publish ||
            (state.total_distance - last_publish_distance) > 30) {

            force_publish = false;
            last_publish_distance = state.total_distance;
            connection->Send(Message{
                player, UserState{state.total_distance, get_direction()}});
        }

        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            state.Update(message.player, message.GetGameState());
        }
    }

    void set_direction(Directions _direction) override {
        force_publish = true;
        return PlayerController::set_direction(_direction);
    }
};
