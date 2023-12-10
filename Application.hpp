#pragma once

#include "Canvas.hpp"
#include "Player.hpp"
#include "controllers/KeyboardPlayerController.hpp"
#include <SFML/Window/Event.hpp>
#include <map>
#include <memory>

class Application {
  private:
    const int SPEED_PER_SEC = 200; //% клетки в секунду
    sf::RenderWindow& window;
    Canvas canvas;
    std::map<sf::Keyboard::Key,
             std::pair<Directions, std::shared_ptr<KeyboardPlayerController>>>
        key_mapping;

  public:
    Application() : canvas{}, window{canvas.window} {}
    void Run();

  private:
    PlayerPtr createLocalPlayer(std::array<sf::Keyboard::Key,4> sf_directions);
    void processEvents();
};