#include "Game.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

GameState& Game::Init() {
    for (auto i = 0; i < players.size(); ++i) {
        init_player(players[i], i, players.size());
    }

    return state;
}

GameState& Game::CalcState(uint64_t total_distance) {
    auto distance = total_distance - state.total_distance;
    state.total_distance = total_distance;

    for (auto player : players) {
        player->Move(distance);
    }

    update_deaths();
    create_food();

    return state;
};

void Game::update_deaths() {
    for (auto snake1 = state.snakes.begin(); snake1 != state.snakes.end();) {
        auto& header1 = (*snake1)->body[0];

        if (check_border(header1)) {
            snake1 = state.snakes.erase(snake1);
            continue;
        }

        bool erased = false;
        for (auto snake2 : state.snakes) {
            if (find_interception(header1, snake2, snake2 == *snake1)) {
                snake1 = state.snakes.erase(snake1);
                erased = true;
                break;
            }
        }

        if (erased) {
            continue;
        }

        if (auto food = food_interception(header1); food != state.foods.end()) {
            state.foods.erase(food);
            (*snake1)->grow_percent += 100;
        }

        ++snake1;
    }
}

bool Game::check_border(SnakeSegment& p) {
    return p.x < 0 || p.y < 0 || p.x >= XSIZE || p.y >= YSIZE;
}

bool Game::find_interception(FieldPoint& p, SnakePtr snake, bool skip_head) {
    for (auto& s : snake->body) {
        if (skip_head) {
            skip_head = false;
            continue;
        }

        if (s == p) {
            return true;
        }
    }

    return false;
}

Foods::const_iterator Game::food_interception(FieldPoint& p) {
    for (auto f = state.foods.begin(); f != state.foods.end(); ++f) {
        if (*f == p) {
            return f;
        }
    }

    return state.foods.end();
}

void Game::create_food() {
    if (state.foods.size() > 1) {
        return;
    }

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist_x(1, XSIZE - 1);
    std::uniform_int_distribution<int> uniform_dist_y(1, YSIZE - 1);

    for (;;) {
        Food f{uniform_dist_x(e1), uniform_dist_y(e1)};
        if (food_interception(f) != state.foods.end()) {
            continue;
        }

        bool find = false;
        for (auto snake1 = state.snakes.begin(); snake1 != state.snakes.end();
             ++snake1) {

            if (find_interception(f, *snake1, false)) {
                find = true;
                break;
            }
        }
        
        if (find) {
            continue;
        }

        state.foods.push_back(f);
        return;
    }
}

void Game::init_player(PlayerPtr player, int index, size_t total) {
    int side_idx = index % 4;
    int side_pos = index / 4 + 1;
    int total_per_side = (int)total / 4;

    if (total % 4 > side_idx) {
        ++total_per_side;
    }

    int x, y, dx, dy;
    Directions currendDirection;
    switch ((Directions)side_idx) {
    case Directions::UP:
        x = XSIZE / (total_per_side + 1) * side_pos;
        y = 0;
        dx = 0;
        dy = 1;
        currendDirection = Directions::DOWN;
        break;

    case Directions::DOWN:
        x = XSIZE / (total_per_side + 1) * side_pos;
        y = YSIZE - 1;
        dx = 0;
        dy = -1;
        currendDirection = Directions::UP;
        break;

    case Directions::LEFT:
        x = 0;
        y = YSIZE / (total_per_side + 1) * side_pos;
        dx = 1;
        dy = 0;
        currendDirection = Directions::RIGTH;
        break;

    case Directions::RIGTH:
        x = XSIZE - 1;
        y = YSIZE / (total_per_side + 1) * side_pos;
        dx = -1;
        dy = 0;
        currendDirection = Directions::LEFT;
        break;

    default:
        throw std::logic_error("invalid direction");
    }

    auto snake = player->snake;
    for (int i = 0; i < INIT_PLAYER_LENGTH; ++i, x += dx, y += dy) {
        snake->body.emplace(snake->body.begin(), x, y);
    }

    // Сегмент змеи на 100% находится в этой ячейке
    snake->percent = 100;
    player->SetDirection(currendDirection);
    state.snakes.push_back(snake);
}
