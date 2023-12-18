#pragma once

#include "../controllers/RemoteServerController.hpp"
#include "../network/Network.hpp"
#include "SfmlNetExchange.hpp"
#include <SFML/Network.hpp>
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>

class SfmlTcpGameClient : public RemotePlayerProvider {
  private:
    const sf::IpAddress ip;
    const unsigned short port;
    std::shared_ptr<sf::TcpSocket> client;

    std::atomic<bool> active = true;
    std::thread acept_thread;

    PlayerControllerPtr conroller;
    std::mutex conroller_access_mutex;

  public:
    SfmlTcpGameClient(const char* ip, unsigned short port)
        : ip{ip}, port{port}, client{std::make_shared<sf::TcpSocket>()} {

        acept_thread = std::thread{[&]() {
            PlayerControllerPtr tmp;
            while (active && !tmp) {
                tmp = acceptClientSync();
            }

            if (tmp) {
                std::lock_guard guard{conroller_access_mutex};               
                conroller = tmp;
            }
        }};
    }

    ~SfmlTcpGameClient() override {
        active = false;
        if (acept_thread.joinable()) {
            acept_thread.join();
        }

        RemotePlayerProvider::~RemotePlayerProvider();
    }

    PlayerControllerPtr AcceptClient() override {
        std::lock_guard guard{conroller_access_mutex};               
        return conroller;

        /*static std::chrono::milliseconds timespan(100);
        // static auto sf_time = sf::milliseconds(500);

        sf::Socket::Status status = client->connect(ip, port);
        if (status == sf::Socket::Done) {
            std::cout << "Client connected" << std::endl;
            return std::make_shared<RemoteServerController>(
                std::make_shared<SfmlNetExchange>(client));
        }

        std::cout << "Client connecting ..., "
                  << "status: " << (int)status << std::endl;

        std::this_thread::sleep_for(timespan);
        return nullptr*/
    }

  private:
    PlayerControllerPtr acceptClientSync() {
        static std::chrono::milliseconds timespan(1000);
        // static auto sf_time = sf::milliseconds(500);

        sf::Socket::Status status = client->connect(ip, port);
        if (status == sf::Socket::Done) {
            std::cout << "Client connected" << std::endl;
            return std::make_shared<RemoteServerController>(
                std::make_shared<SfmlNetExchange>(client));
        }

        std::cout << "Client connecting ..., "
                  << "status: " << (int)status << std::endl;

        std::this_thread::sleep_for(timespan);
        return nullptr;
    }
};