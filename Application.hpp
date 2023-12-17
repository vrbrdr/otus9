#pragma once

#include "Canvas.hpp"
#include "Player.hpp"
#include "controllers/KeyboardPlayerController.hpp"
#include "controllers/RemotePlayerController.hpp"
#include "network/Network.hpp"
#include <SFML/Window/Event.hpp>
#include <map>
#include <memory>

enum class ServerTypes { LOCAL, CLIENT, SERVER };

class Application {
  private:
    const int SPEED_PER_SEC = 50; //% клетки в секунду
    std::unique_ptr<RemotePlayerProvider> remotePlayerProvider;
    Players players;
    Canvas canvas;
    std::shared_ptr<sf::RenderWindow> window;
    std::map<sf::Keyboard::Key,
             std::pair<Directions, std::shared_ptr<KeyboardPlayerController>>>
        key_mapping;

  public:
    Application(ServerTypes type, const char* ip, uint16_t port,
                uint8_t local_players);

    void Run();

  private:
    void createPlayers(uint8_t local_players);
    PlayerPtr createLocalPlayer(uint8_t index,
                                std::array<sf::Keyboard::Key, 4> sf_directions);
    RemotePlayerProvider*
    getRemotePlayerProvider(ServerTypes type, const char* ip, uint16_t port);
    void processEvents();
};