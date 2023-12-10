#pragma once

#include <memory>
#include <vector>

const static int XSIZE = 80;
const static int YSIZE = 40;

enum class Directions {
    LEFT,
    RIGTH,
    UP,
    DOWN,
};

struct FieldPoint {
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

using Food = FieldPoint;
// struct Food : public FieldPoint {
//     Food(int x, int y) : FieldPoint(x, y){};
// };

using Foods = std::vector<Food>;

using SnakeSegment = FieldPoint;
// struct SnakeSegment : public FieldPoint {
//     SnakeSegment(int x, int y) : FieldPoint(x, y){};
// };

using SnakeSegments = std::vector<SnakeSegment>;

struct Snake {
    // head first
    SnakeSegments body;
    Directions direction;
    uint8_t percent;
    uint16_t grow_percent;
};

using SnakePtr = std::shared_ptr<Snake>;
using Snakes = std::vector<SnakePtr>;

struct GameState {
    uint64_t total_distance = 0;
    Snakes snakes;
    Foods foods;
};
