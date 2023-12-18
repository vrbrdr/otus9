#pragma once

#include "../controllers/PlayerController.hpp"
#include "../GameState.hpp"

class InputDevice {
  private:
    std::vector<PlayerControllerPtr> controllers;

  protected:
    void key_pressed(uint8_t controller_id, Directions direction) {
        if (controller_id < controllers.size()) {
            controllers[controller_id]->set_direction(direction);
        }
    }

  public:
    InputDevice(const InputDevice&) = delete;
    InputDevice(InputDevice&&) = delete;
    InputDevice& operator=(const InputDevice&) = delete;
    InputDevice& operator=(InputDevice&&) = delete;
    InputDevice() = default;

    void Register(PlayerControllerPtr controller){
        controllers.push_back(controller);
    }

    virtual ~InputDevice(){}

    virtual void Read() = 0;
};