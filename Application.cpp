#include "Application.hpp"
#include "Game.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

void Application::Run() {
    key_mapping.clear();

    Players players{
       // createLocalPlayer({sf::Keyboard::Key::W, sf::Keyboard::Key::S,
       //                    sf::Keyboard::Key::A, sf::Keyboard::Key::D}),
        createLocalPlayer({sf::Keyboard::Key::Up, sf::Keyboard::Key::Down,
                           sf::Keyboard::Key::Left, sf::Keyboard::Key::Right})};

    Game game{players};

    auto& state = game.Init();
    canvas.draw(state);

    const auto start = std::chrono::high_resolution_clock::now();

    while (window.isOpen()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto total_duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        auto total_distance = SPEED_PER_SEC * total_duration.count() / 1000;

        auto& state = game.CalcState(total_distance);

        canvas.draw(state);
        processEvents();
    }
}

PlayerPtr
Application::createLocalPlayer(std::array<sf::Keyboard::Key, 4> sf_directions) {
    static std::array directions{Directions::UP, Directions::DOWN,
                                 Directions::LEFT, Directions::RIGTH};

    auto controller = std::make_shared<KeyboardPlayerController>();

    for (int i = 0; i < directions.size(); i++) {
        key_mapping.emplace(sf_directions[i],
                            std::pair{directions[i], controller});
    }

    return std::make_shared<Player>(controller);
}

void Application::processEvents() {
    for (sf::Event event{}; window.pollEvent(event); /**/) {
        switch (event.type) {
        case sf::Event::EventType::Closed:
            window.close();
            break;

        case sf::Event::EventType::KeyPressed:
            auto& mapitem = key_mapping.find(event.key.code);
            if (mapitem != key_mapping.end()) {
                mapitem->second.second->KeyPressed(mapitem->second.first);
            }
        }
    }
}
