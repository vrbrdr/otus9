#include "Canvas.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <exception>
#include <iostream>
#include <stdexcept>

Canvas::Canvas()
    : window{{(unsigned int)(XSIZE * scale), (unsigned int)(YSIZE * scale)},
             "Snake game",
             sf::Style::Titlebar | sf::Style::Close,
             sf::ContextSettings(0, 0, 4)} {}

void Canvas::draw(GameState& state) {
    window.clear(sf::Color::White);

    for (auto snake : state.snakes) {
        draw_snake(*snake);
    }

    for (auto& food : state.foods) {
        draw_food(food);
    }

    window.display();
}

void Canvas::draw_snake(Snake& snake) {
    draw_snake_head(snake, snake.body[0]);

    for (int i = 1; i < snake.body.size(); ++i) {
        draw_snake_segment(snake, snake.body[i]);
    }
}

void Canvas::draw_snake_segment(Snake& snake, SnakeSegment& head) {
    float scaled_x = (float)head.x * scale;
    float scaled_y = (float)head.y * scale;

    const sf::Vector2f tl{scaled_x, scaled_y};
    const sf::Vector2f br{scaled_x + scale, scaled_y + scale};

    sf::RectangleShape rect(br - tl);

    rect.setPosition(tl);
    rect.setFillColor(sf::Color::Black);

    window.draw(rect);
}

void Canvas::draw_snake_head(Snake& snake, SnakeSegment& serment) {
    float fscale = (float)scale;
    float fscale4 = (float)scale / 4;

    float scaled_x = serment.x * fscale;
    float scaled_y = serment.y * fscale;

    const sf::Vector2f tl{scaled_x, scaled_y};
    const sf::Vector2f br{scaled_x + scale, scaled_y + scale};

    sf::RectangleShape rect(br - tl);

    rect.setPosition(tl);
    rect.setFillColor(sf::Color::Black);

    window.draw(rect);

    switch (snake.direction) {
    case Directions::UP:
        rect = sf::RectangleShape(sf::Vector2f{fscale, fscale4});
        rect.setPosition(sf::Vector2f{scaled_x, scaled_y});
        break;

    case Directions::DOWN:
        rect = sf::RectangleShape(sf::Vector2f{fscale, fscale4});
        rect.setPosition(sf::Vector2f{scaled_x, scaled_y + fscale - fscale4});
        break;

    case Directions::LEFT:
        rect = sf::RectangleShape(sf::Vector2f{fscale4, fscale});
        rect.setPosition(sf::Vector2f{scaled_x, scaled_y});
        break;

    case Directions::RIGTH:
        rect = sf::RectangleShape(sf::Vector2f{fscale4, fscale});
        rect.setPosition(sf::Vector2f{scaled_x + fscale - fscale4, scaled_y});
        break;

    default:
        throw std::logic_error("invalid direction");
    }

    rect.setFillColor(sf::Color::Red);
    window.draw(rect);
}

void Canvas::draw_food(Food& food) {
    float scaled_x = (float)food.x * scale;
    float scaled_y = (float)food.y * scale;

    const sf::Vector2f tl{scaled_x, scaled_y};
    const sf::Vector2f br{scaled_x + scale, scaled_y + scale};

    sf::RectangleShape rect(br - tl);

    rect.setPosition(tl);
    rect.setFillColor(sf::Color::Green);

    window.draw(rect);
}
