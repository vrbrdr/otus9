#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

const static uint8_t XSIZE = 30;
const static uint8_t YSIZE = 30;

enum class Directions : uint8_t { LEFT, RIGTH, UP, DOWN, UNDEFINED };
enum class Colors : uint8_t { RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, MAX };

#pragma pack(push, 1)
struct FieldPoint final {
    int x;
    int y;

    FieldPoint(int x, int y) noexcept : x{x}, y{y} {}
    FieldPoint(const FieldPoint& source) noexcept : x{source.x}, y{source.y} {}
    FieldPoint(FieldPoint&& source) noexcept : x{source.x}, y{source.y} {}

    FieldPoint& operator=(const FieldPoint& source) noexcept {
        x = source.x;
        y = source.y;
        return *this;
    }

    FieldPoint& operator=(FieldPoint&& source) noexcept {
        x = source.x;
        y = source.y; 
        return *this;
    }

    inline bool operator==(const FieldPoint& other) const noexcept {
        return x == other.x && y == other.y;
    }

    inline bool operator!=(const FieldPoint& other) const noexcept {
        return !operator==(other);
    }
};

struct UserState {
    uint64_t total_distance;
    Directions direction;

    UserState(uint64_t total_distance, Directions direction)
        : total_distance{total_distance}, direction{direction} {}
};
#pragma pack(pop)

using Food = FieldPoint;
using SnakeSegment = FieldPoint;
using Foods = std::vector<Food>;
using SnakeSegments = std::vector<SnakeSegment>;

struct Snake {
    const uint8_t index;
    Directions direction;
    Directions tile_direction;
    Colors color;
    SnakeSegments body;

    Snake(uint8_t index)
        : index{index}, color{(Colors)(index % (uint8_t)Colors::MAX)} {}

    void Grow(int count) {
        auto tail = body[body.size() - 1];
        for (int i = 0; i < count; i++) {
            body.push_back(tail);
        }
    }

    void Die() {
        is_die = true;
    }

    bool IsDie() {
        return is_die;
    }

    void Update(Snake& source) {
        if (index != source.index) {
            throw std::logic_error{"index != source.index"};
        }

        direction = source.direction;
        tile_direction = source.tile_direction;
        body = std::move(source.body);
    }

    inline bool operator==(const Snake& other) const noexcept {
        if (!(index == other.index && direction == other.direction &&
              tile_direction == other.tile_direction && color == other.color &&
              body.size() == other.body.size())) {
            return false;
        }

        for (int i = 0; i < body.size(); ++i) {
            if (body[i] != other.body[i]) {
                return false;
            }
        }

        return true;
    }

    inline bool operator!=(const Snake& other) const noexcept {
        return !operator==(other);
    }

    static Directions TileDirection(SnakeSegment& tile, SnakeSegment& prev) {
        if (tile.x - prev.x > 0) {
            return Directions::LEFT;
        } else if (tile.x - prev.x < 0) {
            return Directions::RIGTH;
        } else if (tile.y - prev.y > 0) {
            return Directions::UP;
        } else if (tile.y - prev.y < 0) {
            return Directions::DOWN;
        } else {
            return Directions::UNDEFINED;
        }
    }

  private:
    bool is_die = false;
};

using SnakePtr = std::shared_ptr<Snake>;
using Snakes = std::vector<SnakePtr>;

struct GameState {
    uint64_t total_distance = 0;
    Snakes snakes;
    Foods foods;

    inline uint64_t percent() {
        return total_distance % 100;
    }

    void Update(GameState&& source) {
        foods.clear();

        total_distance = source.total_distance;
        foods = std::move(source.foods);

        for (int i = 0; i < snakes.size(); ++i) {
            snakes[i]->Update(*(source.snakes[i]));
        }
    }
};
