#pragma once

#include "../device/Device.hpp"
#include "SfmlDrawDevice.hpp"
#include "SfmlInputDevice.hpp"
#include "SfmlTcpGameClient.hpp"
#include "SfmlTcpGameServer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class SfmlDevice : public Device {
  private:
    std::shared_ptr<sf::RenderWindow> window;
    const int scale = 20;

  public:
    SfmlDevice(std::string header) {
        window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode{(unsigned int)(XSIZE * scale),
                          (unsigned int)(YSIZE * scale)},
            header, sf::Style::Titlebar | sf::Style::Close,
            sf::ContextSettings(0, 0, 4));
    }

    bool alive() override {
        return window->isOpen();
    }

    std::shared_ptr<DrawDevice> get_draw() override {
        static auto device = std::make_shared<SfmlDrawDevice>(window);
        return device;
    }

    std::shared_ptr<InputDevice> get_input() override {
        static auto device = std::make_shared<SfmlInputDevice>(window);
        return device;
    }

    std::shared_ptr<RemotePlayerProvider>
    GetNetworkPlayerProvider(ServerTypes type, const char* ip, uint16_t port) override{
        switch (type) {
        case ServerTypes::LOCAL:
            return nullptr;

        case ServerTypes::SERVER:
            return std::make_shared<SfmlTcpGameServer>(ip, port);

        case ServerTypes::CLIENT:
            return std::make_shared<SfmlTcpGameClient>(ip, port);

        default:
            throw std::logic_error("Invalid ServerTypes value");
        }
    }
};