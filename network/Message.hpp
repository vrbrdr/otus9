#pragma once

#include "../GameState.hpp"
#include <stdexcept>
#include <stdint.h>

#pragma pack(push, 1)
const uint32_t headerPrefix = 0x01020304;

struct MessageHeader {
    uint32_t prefix;
    uint16_t totalLenght; // Длина всего сообщения вместе с загловками
    uint8_t player;

    MessageHeader(uint16_t totalLenght, uint8_t player)
        : prefix{headerPrefix}, totalLenght{totalLenght}, player{player} {}
};

struct MessageFooter {
    uint32_t crc32;
};

struct Message {
    unsigned char* message;
    uint8_t player;
    uint16_t lenght;

    Message& operator=(const Message& src) = delete;
    Message& operator=(Message&& src) {
        if (&src != this) {
            delete[] message;
            message = src.message;
            lenght = src.lenght;

            src.message = nullptr;
            src.lenght = 0;
        }

        return *this;
    };

    Message(const Message& src) = delete;
    Message(Message&& src)
        : player{src.player}, lenght{src.lenght}, message{src.message} {
        src.message = nullptr;
        src.lenght = 0;
    }

    Message(uint8_t player, const UserState& state)
        : message{new unsigned char[lenght]}, player{player},
          lenght{sizeof(MessageHeader) + sizeof(UserState) +
                 sizeof(MessageFooter)} {

        memcpy(message + sizeof(MessageHeader), &state, sizeof(state));

        fill();
    }

    Message(uint8_t player, GameState& state) : player{player} {
        std::vector<SnakeInfo> snakes_infos;
        std::vector<SnakeSegment> snakes_segments;

        for (auto s : state.snakes) {
            snakes_infos.emplace_back(*(s.get()));
            for (auto& sbs : s->body) {
                snakes_segments.emplace_back(sbs);
            }
        }

        MessageStateHeader state_header{state};

        lenght = (uint16_t)(sizeof(MessageHeader) + sizeof(state_header) +
                            sizeof(state.total_distance) +
                            sizeof(Food) * state.foods.size() +
                            sizeof(SnakeInfo) * snakes_infos.size() +
                            sizeof(SnakeSegment) * snakes_segments.size() +
                            sizeof(MessageFooter));

        message = new unsigned char[lenght];

        size_t copy_index = sizeof(MessageHeader);

        memcpy(message + copy_index, &state_header, sizeof state_header);
        copy_index += sizeof state_header;

        memcpy(message + copy_index, &state.total_distance,
               sizeof state.total_distance);
        copy_index += sizeof state.total_distance;

        if (state_header.FoodsCount > 0) {
            memcpy(message + copy_index, &state.foods[0],
                   sizeof(Food) * state.foods.size());
            copy_index += sizeof(Food) * state.foods.size();
        }

        if (state_header.SnakesCount > 0) {
            memcpy(message + copy_index, &snakes_infos[0],
                   sizeof(SnakeInfo) * snakes_infos.size());
            copy_index += sizeof(SnakeInfo) * snakes_infos.size();

            memcpy(message + copy_index, &snakes_segments[0],
                   sizeof(SnakeSegment) * snakes_segments.size());
        }

        fill();
    }

    Message(unsigned char* _message, uint16_t lenght) : lenght{lenght} {
        message = new unsigned char[lenght];
        memcpy(message, _message, lenght);
        auto crc32 = getCrc();
        auto header = (MessageHeader*)message;
        auto footer =
            (MessageFooter*)(message + lenght - sizeof(MessageFooter));

        player = header->player;

        if (header->prefix != headerPrefix) {
            throw new std::invalid_argument("prefix fail");
        }

        if (header->totalLenght != lenght) {
            throw new std::invalid_argument("packet lenth fail");
        }

        if (footer->crc32 != crc32) {
            throw new std::invalid_argument("crc fail");
        }
    }

    Message() : message{nullptr}, lenght{0} {}

    ~Message() {
        delete[] message;
    }

    bool IsEmpty() const noexcept {
        return lenght == 0;
    }

    UserState GetUserState() const {
        return *((UserState*)(message + sizeof(MessageHeader)));
    }

    GameState GetGameState() const {
        GameState gs;

        size_t index = sizeof(MessageHeader);

        auto header = (MessageStateHeader*)(message + index);
        index += sizeof(MessageStateHeader);

        gs.total_distance = *((decltype(gs.total_distance)*)(message + index));
        index += sizeof(gs.total_distance);

        if (header->FoodsCount > 0) {
            Food* foods = (Food*)(message + index);
            index += sizeof(Food) * header->FoodsCount;

            for (int i = 0; i < header->FoodsCount; ++i) {
                gs.foods.push_back(foods[i]);
            }
        }

        if (header->SnakesCount > 0) {
            SnakeInfo* sinfos = (SnakeInfo*)(message + index);
            index += sizeof(SnakeInfo) * header->SnakesCount;

            SnakeSegment* segments = (SnakeSegment*)(message + index);
            int idx = 0;
            for (int snake_idx = 0; snake_idx < header->SnakesCount;
                 ++snake_idx) {
                auto snake = std::make_shared<Snake>(sinfos[snake_idx].index);
                gs.snakes.push_back(snake);

                snake->direction = sinfos[snake_idx].direction;
                snake->tile_direction = sinfos[snake_idx].tile_direction;

                for (int segment = 0; segment < sinfos[snake_idx].size;
                     ++segment, ++idx) {
                    snake->body.push_back(segments[idx]);
                }
            }
        }

        return gs;
    }

  private:
    uint32_t getCrc() {
        uint32_t res = 0;
        for (int i = 0; i + sizeof(MessageFooter) < lenght; ++i) {
            res += message[i];
        }

        return res;
    }

    void fill() {
        auto header = (MessageHeader*)message;
        header->prefix = headerPrefix;
        header->totalLenght = lenght;
        header->player = player;

        auto footer =
            (MessageFooter*)(message + lenght - sizeof(MessageFooter));

        footer->crc32 = getCrc();
    }

    struct MessageStateHeader {
        size_t SnakesCount;
        size_t FoodsCount;
        MessageStateHeader(GameState& state)
            : SnakesCount{state.snakes.size()}, FoodsCount{state.foods.size()} {
        }
    };

    struct SnakeInfo {
        uint8_t index;
        Directions direction;
        Directions tile_direction;
        size_t size;

        SnakeInfo(Snake& snake)
            : index{snake.index}, direction{snake.direction},
              tile_direction{snake.tile_direction}, size{snake.body.size()} {}
    };
};

#pragma pack(pop)