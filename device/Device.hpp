#pragma once

#include "../Consts.hpp"
#include "DrawDevice.hpp"
#include "InputDevice.hpp"
#include <memory>
#include <string>

class Device {
  public:
    virtual ~Device() {}

    virtual bool alive() = 0;
    virtual std::shared_ptr<DrawDevice> get_draw() = 0;
    virtual std::shared_ptr<InputDevice> get_input() = 0;
    virtual std::shared_ptr<RemotePlayerProvider>
    GetNetworkPlayerProvider(ServerTypes type, const char* ip,
                             uint16_t port) = 0;
};