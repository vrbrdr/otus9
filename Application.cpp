#include "Application.hpp"
#include "Game.hpp"
#include "controllers/EmptyController.hpp"
#include "network/Network.hpp"
#include "network/SfmtNetwork.hpp"
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
    : type{type}, canvas{make_header(type, ip, port)} {

    window = canvas.window;

    remotePlayerProvider = std::shared_ptr<RemotePlayerProvider>(
        getRemotePlayerProvider(ip, port));

    if (type == ServerTypes::CLIENT) {
        createClientPlayers();
    } else {
        createServerPlayers(local_players);
    }
}

void Application::Run() {
    Game game{type != ServerTypes::CLIENT, players};

    auto& state = game.Init();

    canvas.draw(state);

    const auto start = std::chrono::high_resolution_clock::now();
    uint64_t prev_distance = 0;

    while (window->isOpen()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto total_duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        auto total_distance = SPEED_PER_SEC * total_duration.count() / 1000;

        state = game.CalcState(total_distance - prev_distance);
        prev_distance = total_distance;

        canvas.draw(state);
        processEvents();
    }
}

void Application::createServerPlayers(uint8_t local_players) {
    static std::array<sf::Keyboard::Key, 4> p1_keys = {
        sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right};

    static std::array<sf::Keyboard::Key, 4> p2_keys = {
        sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A,
        sf::Keyboard::Key::D};

    switch (local_players) {
    case 0:
        break;

    case 2:
        players.push_back(createLocalPlayer(1, p2_keys));
        break;

    case 1:
        players.push_back(createLocalPlayer(0, p1_keys));
        break;

    default:
        throw std::logic_error("Invalid local users count");
    }

    if (!remotePlayerProvider) {
        return;
    }

    //!!!!!! ждем клиентов в течении фиксированного таймаута
    const int timeoutsec = 200;
    const auto timeout = std::chrono::high_resolution_clock::now() +
                         std::chrono::seconds{timeoutsec};

    while (window->isOpen() &&
           std::chrono::high_resolution_clock::now() < timeout) {
        auto controller = remotePlayerProvider->AcceptClient();

        if (controller) {
            auto pidx = controller->Connect(local_players++);
            auto p = std::make_shared<Player>(
                pidx, std::shared_ptr<PlayerController>(controller));
            players.push_back(p);
            return;
        }

        processEvents();
    }
}

void Application::createClientPlayers() {
    static std::array<sf::Keyboard::Key, 4> p1_keys = {
        sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right};

    if (!remotePlayerProvider) {
        throw std::logic_error("remotePlayerProvider == null");
    }

    //!!!!!! ждем клиентов в течении фиксированного таймаута
    const int timeoutsec = 200;
    const auto timeout = std::chrono::high_resolution_clock::now() +
                         std::chrono::seconds{timeoutsec};

    while (window->isOpen() &&
           std::chrono::high_resolution_clock::now() < timeout) {
        auto controller = remotePlayerProvider->AcceptClient();

        if (controller) {
            auto pidx = controller->Connect(0);
            auto p = std::make_shared<Player>(
                pidx, std::shared_ptr<PlayerController>(controller));

            registerKeyboardController(controller, p1_keys);
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
                                  std::shared_ptr<PlayerController>(
                                      std::make_shared<EmptyController>()));

                    new_player->SetDirection(s->direction);
                    new_player->snake = s;
                    players.push_back(new_player);                    
                }
                return;
            }
        }

        processEvents();
    }
}

PlayerPtr
Application::createLocalPlayer(uint8_t index,
                               std::array<sf::Keyboard::Key, 4> sf_directions) {
    auto controller = std::make_shared<KeyboardPlayerController>();
    registerKeyboardController(controller, sf_directions);

    return std::make_shared<Player>(index, controller);
}

void Application::registerKeyboardController(
    PlayerControllerPtr controller,
    std::array<sf::Keyboard::Key, 4> sf_directions) {

    static std::array directions{Directions::UP, Directions::DOWN,
                                 Directions::LEFT, Directions::RIGTH};

    for (int i = 0; i < directions.size(); i++) {
        key_mapping.emplace(sf_directions[i],
                            std::pair{directions[i], controller});
    }
}

RemotePlayerProvider* Application::getRemotePlayerProvider(const char* ip,
                                                           uint16_t port) {
    switch (type) {
    case ServerTypes::LOCAL:
        return nullptr;

    case ServerTypes::SERVER:
        return new SftmTcpGameServer(ip, port);

    case ServerTypes::CLIENT:
        return new SftmTcpGameClient(ip, port);

    default:
        throw std::logic_error("Invalid ServerTypes value");
    }
}

void Application::processEvents() {
    for (sf::Event event{}; window->pollEvent(event); /**/) {
        switch (event.type) {
        case sf::Event::EventType::Closed:
            window->close();
            break;

        case sf::Event::EventType::KeyPressed:
            const auto& mapitem = key_mapping.find(event.key.code);
            if (mapitem != key_mapping.end()) {
                mapitem->second.second->set_direction(mapitem->second.first);
            }
        }
    }
}
