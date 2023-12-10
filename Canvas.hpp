#pragma once

#include "GameState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class Canvas {
  public:
    Canvas();
    void draw(GameState& state);
    sf::RenderWindow window;

  private:
    inline static int scale = 20;
    void draw_snake(Snake& snake);
    void draw_snake_segment(Snake& snake, SnakeSegment& point);
    void draw_snake_head(Snake& snake, SnakeSegment& point);
    void draw_food(Food& food);
};