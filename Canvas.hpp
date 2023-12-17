#pragma once

#include "GameState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>

class Canvas {
  public:
    Canvas(std::string header);
    void draw(GameState& state);
    std::shared_ptr<sf::RenderWindow> window;

  private:
    inline static int scale = 20;
    void draw_snake(Snake& snake, size_t percent);
    void draw_snake_segment(Snake& snake, SnakeSegment& point, size_t percent,
                            Directions tile_directon);
    void draw_snake_head(Snake& snake, SnakeSegment& point, size_t percent,
                         Directions tile_directon);
    void draw_food(Food& food);
    void move_back(Directions direction, uint64_t percent, sf::Shape* shape);
};
