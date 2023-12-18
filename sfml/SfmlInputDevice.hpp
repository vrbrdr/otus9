#pragma once

#include "../device/InputDevice.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <array>
#include <memory>
#include <vector>
#include <map>

class SfmlInputDevice : public InputDevice {
  private:
    const std::shared_ptr<sf::RenderWindow> window;
    std::map<sf::Keyboard::Key, std::pair<Directions, uint8_t>> key_mapping;

    void set_mapping(uint8_t controller_id,
                     std::array<sf::Keyboard::Key, 4> sf_directions,
                     std::array<Directions, 4> directions);

  public:
    SfmlInputDevice(const SfmlInputDevice&) = delete;
    SfmlInputDevice(SfmlInputDevice&&) = delete;
    SfmlInputDevice& operator=(const SfmlInputDevice&) = delete;
    SfmlInputDevice& operator=(SfmlInputDevice&&) = delete;

    SfmlInputDevice(std::shared_ptr<sf::RenderWindow> window);
    void Read() override;
};
