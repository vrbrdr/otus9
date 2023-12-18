#pragma once

#include "../network/Message.hpp"
#include "../network/Network.hpp"
#include "RemoteController.hpp"

class RemotePlayerController : public RemoteController {
  private:
    uint64_t last_publish_distance = 0;

  public:
    RemotePlayerController(std::shared_ptr<NetExchange> connection)
        : RemoteController(connection) {}

    ~RemotePlayerController() override {
        RemoteController::~RemoteController();
    }

    std::shared_ptr<GameState> GetState() override {
        //!!!!!! подумать об использовании total_distance;
        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            RemoteController::set_direction(message.GetUserState().direction);
        }

        return nullptr;
    };

    void SendState(GameState& state) override {
        if (state.force_exchange ||
            (state.total_distance - last_publish_distance) > 30) {

            last_publish_distance = state.total_distance;

            connection->Send(Message{player, state});
        }
    };

    virtual Directions get_direction() override {
        return RemoteController::get_direction();
    }
};