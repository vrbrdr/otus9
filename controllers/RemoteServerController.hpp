#pragma once

#include "../network/Network.hpp"
#include "RemoteController.hpp"

class RemoteServerController : public RemoteController {
  private:
    bool force_publish = true;
    uint64_t last_publish_distance = 0;

  public:
    RemoteServerController(std::shared_ptr<NetExchange> connection)
        : RemoteController(connection) {}

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
    virtual std::shared_ptr<GameState> GetState() override{
        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            return message.GetGameState();
        }

        return nullptr;
    };

    virtual void SendState(GameState& state) override{
        if (force_publish ||
            (state.total_distance - last_publish_distance) > 5) {

            force_publish = false;
            auto current_direction = get_direction();
            if (current_direction == Directions::UNDEFINED) {
                return;
            }
            last_publish_distance = state.total_distance;
            connection->Send(Message{
                player, UserState{state.total_distance, get_direction()}});
        }      
    };

    void set_direction(Directions _direction) override {
        force_publish = true;
        return PlayerController::set_direction(_direction);
    }
};
