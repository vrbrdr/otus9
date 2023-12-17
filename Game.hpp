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
    bool is_local;

  public:
    Game(bool is_local, const Players& players)
        : is_local{is_local}, players{players} {};

    GameState& Init();
    GameState& CalcState(uint64_t distance);    

  private:
    void update_collisions();
    bool check_border(SnakeSegment& p);
    bool find_interception(FieldPoint& p, Snake &Snake, bool skip_head);
    Foods::const_iterator food_interception(FieldPoint& p);
    void create_food();
    void init_player(Player &player, int index, size_t total);
    void move_snake(Snake& snake, uint64_t distance);
    void exchange();
};
