#pragma once

#include "RemoteController.hpp"
#include <map>
#include <memory>
#include <stdint.h>

class RemoteServerController : public RemoteController {
  private:
    bool force_publish = true;
    uint64_t last_publish_distance = 0;
    std::map<uint8_t, PlayerControllerPtr> slave_controlers;

  public:
    RemoteServerController(std::shared_ptr<NetExchange> connection)
        : RemoteController(connection) {}

    ~RemoteServerController() override {
        RemoteController::~RemoteController();
    }

    uint8_t Connect(uint8_t) override {
        for (;;) {
            auto const& message = connection->Receive();
            if (!message.IsEmpty()) {
                player = message.player;
                return RemoteController::Connect(player);
            }
        }
    }

    virtual std::shared_ptr<GameState> GetState() override {
        auto const& message = connection->Receive();
        if (!message.IsEmpty()) {
            return message.GetGameState();
        }

        return nullptr;
    }

    virtual void SendState(GameState& state) override {
        if (force_publish || state.force_exchange ||
            (state.total_distance - last_publish_distance) > 30) {

            force_publish = false;
            last_publish_distance = state.total_distance;

            auto current_direction = get_direction();
            if (current_direction == Directions::UNDEFINED) {
                return;
            }

            connection->Send(Message{
                player, UserState{state.total_distance, get_direction()}});
        }
    }

    void set_direction(Directions _direction) override {
        force_publish = true;
        return PlayerController::set_direction(_direction);
    }

  private:
    /*
          PlayerControllerPtr GetRemotePlayerController(uint8_t index) {
            auto controller = std::make_shared<EmptyController>();
            slave_controlers[index] = controller;
            return controller;
        }

        class EmptyController : public PlayerController {
          public:
            EmptyController() = default;

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
        };

    */
};
