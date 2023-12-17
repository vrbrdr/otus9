#include "Canvas.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <exception>
#include <iostream>
#include <stdexcept>

Canvas::Canvas(std::string header)
    : window{std::make_shared<sf::RenderWindow>(
          sf::VideoMode{(unsigned int)(XSIZE * scale),
                        (unsigned int)(YSIZE * scale)},
          header, sf::Style::Titlebar | sf::Style::Close,
          sf::ContextSettings(0, 0, 4))} {}

void Canvas::draw(GameState& state) {
    window->clear(sf::Color::White);

    for (auto snake : state.snakes) {
        if (!snake->IsDie()) {
            draw_snake(*snake, 100 - state.percent());
        }
    }

    for (auto& food : state.foods) {
        draw_food(food);
    }

    window->display();
}

void Canvas::draw_snake(Snake& snake, size_t reverce_percent) {
    if(snake.body.size() == 0){
        return;
        //throw std::logic_error("snake.body.size() == 0");
    }

    Directions dir;
    for (int i = 0; i + 1 < snake.body.size(); ++i) {
        auto& seg = snake.body[i];
        dir = snake.TileDirection(seg, snake.body[i + 1]);

        if (i == 0) {
            draw_snake_head(snake, seg, reverce_percent, dir);
        } else {
            draw_snake_segment(snake, seg, reverce_percent, dir);
        }
    }

    draw_snake_segment(snake, snake.body[snake.body.size() - 1],
                       reverce_percent, snake.tile_direction);
}

void Canvas::draw_snake_segment(Snake& snake, SnakeSegment& head,
                                size_t reverce_percent,
                                Directions tile_directon) {

    float scaled_x = (float)head.x * scale;
    float scaled_y = (float)head.y * scale;

    const sf::Vector2f tl{scaled_x, scaled_y};
    const sf::Vector2f br{scaled_x + scale, scaled_y + scale};

    sf::RectangleShape rect(br - tl);

    rect.setPosition(tl);
    switch (snake.color) {
    case Colors::RED:
        rect.setFillColor(sf::Color::Red);
        break;

    case Colors::GREEN:
        rect.setFillColor(sf::Color::Green);
        break;

    case Colors::BLUE:
        rect.setFillColor(sf::Color::Blue);
        break;

    case Colors::YELLOW:
        rect.setFillColor(sf::Color::Yellow);
        break;

    case Colors::MAGENTA:
        rect.setFillColor(sf::Color::Magenta);
        break;

    case Colors::CYAN:
        rect.setFillColor(sf::Color::Cyan);
        break;

    default:
        rect.setFillColor(sf::Color::Black);
    }

    if (tile_directon != Directions::UNDEFINED) {
        move_back(tile_directon, reverce_percent, &rect);
    }

    window->draw(rect);
}

void Canvas::draw_snake_head(Snake& snake, SnakeSegment& serment,
                             size_t reverce_percent, Directions tile_directon) {
    float fscale = (float)scale;
    float fscale4 = (float)scale / 4;

    float scaled_x = serment.x * fscale;
    float scaled_y = serment.y * fscale;

    const sf::Vector2f tl{scaled_x, scaled_y};
    const sf::Vector2f br{scaled_x + scale, scaled_y + scale};

    sf::RectangleShape rect(br - tl);

    rect.setPosition(tl);
    rect.setFillColor(sf::Color::Black);

    move_back(tile_directon, reverce_percent, &rect);
    window->draw(rect);

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
    move_back(tile_directon, reverce_percent, &rect);
    window->draw(rect);
}

void Canvas::draw_food(Food& food) {
    float scaled_x = (float)food.x * scale;
    float scaled_y = (float)food.y * scale;

    const sf::Vector2f tl{scaled_x, scaled_y};
    const sf::Vector2f br{scaled_x + scale, scaled_y + scale};

    sf::RectangleShape rect(br - tl);

    rect.setPosition(tl);
    rect.setFillColor(sf::Color::Green);

    window->draw(rect);
}

void Canvas::move_back(Directions direction, uint64_t reverce_percent,
                       sf::Shape* shape) {
    auto pos = shape->getPosition();

    switch (direction) {
    case Directions::UP:
        pos.y -= scale * reverce_percent / 100;
        break;

    case Directions::DOWN:
        pos.y += scale * reverce_percent / 100;
        break;

    case Directions::LEFT:
        pos.x -= scale * reverce_percent / 100;
        break;

    case Directions::RIGTH:
        pos.x += scale * reverce_percent / 100;
        break;

    default:
        throw std::logic_error("invalid direction");
    }

    shape->setPosition(pos);
}
