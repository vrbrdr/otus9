#pragma once

#include "GameState.hpp"
#include "Player.hpp"
#include "controllers/KeyboardPlayerController.hpp"
#include <vector>
#include <random>

class Game {
  private:
    const int INIT_PLAYER_LENGTH = 6;
    GameState state;
    Players players;

  public:
    Game(Players& players) : players{players} {};
    GameState& Init();
    GameState& CalcState(uint64_t distance);

  private:
    void update_deaths();
    bool check_border(SnakeSegment& p);
    bool find_interception(SnakeSegment& p, SnakePtr Snake, bool skip_head);
    Foods::const_iterator food_interception(SnakeSegment& p);
    void create_food();
    void init_player(PlayerPtr player, int index, size_t total);
    void move_snake(Snake& snake, uint64_t distance);
};
