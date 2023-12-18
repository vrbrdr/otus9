#include "SfmlInputDevice.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <array>
#include <memory>
#include <stdexcept>

SfmlInputDevice::SfmlInputDevice(std::shared_ptr<sf::RenderWindow> window)
    : window{window} {

    static std::array p1_keys = {sf::Keyboard::Key::Up, sf::Keyboard::Key::Down,
                                 sf::Keyboard::Key::Left,
                                 sf::Keyboard::Key::Right};

    static std::array p2_keys = {sf::Keyboard::Key::W, sf::Keyboard::Key::S,
                                 sf::Keyboard::Key::A, sf::Keyboard::Key::D};

    static std::array directions{Directions::UP, Directions::DOWN,
                                 Directions::LEFT, Directions::RIGTH};

        set_mapping(0, p1_keys, directions);
        set_mapping(1, p2_keys, directions);
}

void SfmlInputDevice::set_mapping(
    uint8_t controller_id, std::array<sf::Keyboard::Key, 4> sf_directions,
    std::array<Directions, 4> directions) {
    for (int i = 0; i < directions.size(); i++) {
        key_mapping.emplace(sf_directions[i],
                            std::pair{directions[i], controller_id});
    }
}

void SfmlInputDevice::Read() {
    for (sf::Event event{}; window->pollEvent(event);) {
        switch (event.type) {
        case sf::Event::EventType::Closed:
            window->close();
            break;

        case sf::Event::EventType::KeyPressed:
            const auto& mapitem = key_mapping.find(event.key.code);
            if (mapitem != key_mapping.end()) {
                key_pressed(mapitem->second.second, mapitem->second.first);
            }
        }
    }
}
