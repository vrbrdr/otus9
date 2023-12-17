#pragma once

#include "../network/Message.hpp"
#include "../network/Network.hpp"
#include "RemoteController.hpp"

class RemotePlayerController : public RemoteController {
  private:
    uint64_t last_publish_distance = 0;

  public:
    RemotePlayerController(std::unique_ptr<NetExchange> connection)
        : RemoteController(std::move(connection)) {}

    ~RemotePlayerController() override {
        RemoteController::~RemoteController();
    }

    virtual void Exchange( GameState& state) override {
        if ((state.total_distance - last_publish_distance) > 30) {
            last_publish_distance = state.total_distance;

            connection->Send(Message{player, state});
        }

        //!!!!!! подумать об использовании total_distance;
        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            RemoteController::set_direction(message.GetUserState().direction);
        }
    };

    virtual Directions get_direction() override {
        return RemoteController::get_direction();
    }
};