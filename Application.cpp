#include "Application.hpp"
#include "Game.hpp"
#include "controllers/EmptyController.hpp"
#include "network/Network.hpp"
#include "sfml/SfmlDevice.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace {
    std::string make_header(ServerTypes type, const char* ip, uint16_t port) {
        if (type == ServerTypes::CLIENT) {

            return std::string("CLIENT\\") + ip + ":" + std::to_string(port);
        }
        if (type == ServerTypes::SERVER) {
            return std::string("SERVER\\") + ip + ":" + std::to_string(port);
        } else {
            return std::string("LOCAL");
        }
    }

} // namespace

Application::Application(ServerTypes type, const char* ip, uint16_t port,
                         uint8_t local_players)
    : type{type} {

    device = std::make_unique<SfmlDevice>(make_header(type, ip, port));
    draw_device = device->get_draw();
    input_device = device->get_input();

    remotePlayerProvider = device->GetNetworkPlayerProvider(type, ip, port);

    if (type == ServerTypes::CLIENT) {
        createClientPlayers();
    } else {
        createLocalPlayers(local_players);
        createServerPlayers();
    }
}

void Application::Run() {
    Game game{type != ServerTypes::CLIENT, players};

    auto& state = game.Init();

    draw_device->draw(state);

    const auto start = std::chrono::high_resolution_clock::now();
    uint64_t prev_distance = 0;

    while (device->alive()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto total_duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        auto total_distance = SPEED_PER_SEC * total_duration.count() / 1000;

        state = game.CalcState(total_distance - prev_distance);       
        prev_distance = total_distance;

        
        draw_device->draw(state);
        processEvents();
    }
}
void Application::createLocalPlayers(uint8_t local_players) {
    switch (local_players) {
    case 0:
        break;

    case 2:
        players.push_back(createLocalPlayer(0));
        players.push_back(createLocalPlayer(1));
        break;

    case 1:
        players.push_back(createLocalPlayer(0));
        break;

    default:
        throw std::logic_error("Invalid local users count");
    }
}

void Application::createServerPlayers() {

    if (!remotePlayerProvider) {
        return;
    }

    uint8_t players_cout = (uint8_t)players.size();

    //!!!!!! ждем клиентов в течении фиксированного таймаута
    const int timeoutsec = 200;
    const auto timeout = std::chrono::high_resolution_clock::now() +
                         std::chrono::seconds{timeoutsec};

    while (device->alive() &&
           std::chrono::high_resolution_clock::now() < timeout) {
        auto controller = remotePlayerProvider->AcceptClient();

        if (controller) {
            auto pidx = controller->Connect(players_cout++);
            auto p =
                std::make_shared<Player>(pidx, PlayerControllerPtr(controller));
            players.push_back(p);
            return;
        }

        processEvents();
    }
}

void Application::createClientPlayers() {
    if (!remotePlayerProvider) {
        throw std::logic_error("remotePlayerProvider == null");
    }

    //!!!!!! ждем клиентов в течении фиксированного таймаута
    const int timeoutsec = 200;
    const auto timeout = std::chrono::high_resolution_clock::now() +
                         std::chrono::seconds{timeoutsec};

    while (device->alive() &&
           std::chrono::high_resolution_clock::now() < timeout) {
        auto controller = remotePlayerProvider->AcceptClient();

        if (controller) {
            auto pidx = controller->Connect(0);
            auto p =
                std::make_shared<Player>(pidx, PlayerControllerPtr(controller));

            registerKeyboardController(controller);
            for (;;) {
                auto gs = controller->GetState();
                if (!gs) {
                    processEvents();
                    continue;
                }

                for (auto& s : gs->snakes) {
                    auto new_player =
                        (s->index == pidx)
                            ? p
                            : std::make_shared<Player>(
                                  s->index,
                                  PlayerControllerPtr(
                                      std::make_shared<EmptyController>(s)));

                    new_player->SetDirection(s->get_direction());
                    new_player->snake = s;
                    players.push_back(new_player);
                }
                return;
            }
        }

        processEvents();
    }
}

PlayerPtr Application::createLocalPlayer(uint8_t index) {
    auto controller = std::make_shared<KeyboardPlayerController>();
    registerKeyboardController(controller);

    return std::make_shared<Player>(index, controller);
}

void Application::registerKeyboardController(PlayerControllerPtr controller) {
    input_device->Register(controller);
}

void Application::processEvents() {
    input_device->Read();
}
