#pragma once

#include "../controllers/RemotePlayerController.hpp"
#include "../network/Network.hpp"
#include "SfmlNetExchange.hpp"
#include <SFML/Network.hpp>
#include <memory>

class SfmlTcpGameServer : public RemotePlayerProvider {
  private:
    const sf::IpAddress ip;
    const unsigned short port;
    sf::TcpListener listener;
    std::shared_ptr<sf::TcpSocket> client;

  public:
    SfmlTcpGameServer(const char* ip, unsigned short port)
        : ip{ip}, port{port} {
        listener.setBlocking(false);

        std::cout << "create SftmTcpGameServer(" << ip << ":" << port << ")"
                  << std::endl;

        if (listener.listen(port) != sf::Socket::Done) {
            throw net_exception();
        }
    }

    ~SfmlTcpGameServer() override {
        listener.close();
        RemotePlayerProvider::~RemotePlayerProvider();
    }

    PlayerControllerPtr AcceptClient() override {
        if (!client) {
            client = std::make_shared<sf::TcpSocket>();
        }

        switch (listener.accept(*client)) {
        case sf::Socket::Done:
            std::cout << "Connection received" << std::endl;
            return std::make_shared<RemotePlayerController>(
                std::make_shared<SfmlNetExchange>(client));

        case sf::Socket::NotReady:
            return nullptr;

        default:
            throw net_exception();
        }
    }
};