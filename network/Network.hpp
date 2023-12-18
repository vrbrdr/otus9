#pragma once

#include "../controllers/PlayerController.hpp"
#include "Message.hpp"
#include <stdexcept>

class net_exception : public std::exception {};

class NetExchange {
  public:
    virtual void Send(const Message& msg) = 0;
    virtual Message Receive() = 0;
    virtual ~NetExchange() {}
};

class RemotePlayerProvider {
  public:
    virtual PlayerControllerPtr AcceptClient() = 0;
    virtual ~RemotePlayerProvider(){};
};