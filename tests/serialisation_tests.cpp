#include "../GameState.hpp"
#include "../network/Message.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>

namespace {

    TEST(serialization, GameState_Empty) {
        GameState s_send{};
        s_send.total_distance = 0x1234567890876543;
        Message m_send{12, s_send};
        Message m_recv{m_send.message, m_send.lenght};
        auto s_recv = m_recv.GetGameState();

        EXPECT_EQ(m_recv.lenght, m_send.lenght);
        EXPECT_EQ(m_recv.player, m_send.player);
        EXPECT_EQ(s_send.total_distance, s_recv.total_distance);
        EXPECT_EQ(s_send.foods.size(), s_recv.foods.size());
        EXPECT_EQ(s_send.snakes.size(), s_recv.snakes.size());

        for (int i = 0; s_send.snakes.size(); ++i) {
            EXPECT_EQ(s_send.snakes[i], s_recv.snakes[i]);
        }
    }

    TEST(serialization, GameState_Fill) {
        GameState s_send{};
        s_send.total_distance = 0x9087654312345678;
        s_send.foods.emplace_back(0, 0);
        s_send.foods.emplace_back(100, 200);

        for (int i = 0; i < 3; ++i) {
            auto s = std::make_shared<Snake>(i + 100);
            s->direction = Directions::LEFT;
            s->tile_direction = Directions::RIGTH;

            for (int j = 0; j < i * 3 + 5; ++j) {
                s->body.emplace_back(i + j, i - j);
            }

            s_send.snakes.push_back(s);
        }

        Message m_send{12, s_send};
        Message m_recv{m_send.message, m_send.lenght};
        m_send.message = nullptr;

        auto s_recv = m_recv.GetGameState();

        EXPECT_EQ(m_recv.lenght, m_send.lenght);
        EXPECT_EQ(m_recv.player, m_send.player);
        EXPECT_EQ(s_send.total_distance, s_recv.total_distance);
        EXPECT_EQ(s_send.foods.size(), s_recv.foods.size());
        EXPECT_EQ(s_send.snakes.size(), s_recv.snakes.size());

        for (int i = 0; i < s_send.snakes.size(); ++i) {
            auto& s1 = *(s_send.snakes[i]);
            auto& s2 = *(s_recv.snakes[i]);
            auto res = s1.operator==(s2);
            EXPECT_TRUE(res);
        }
    }

    TEST(serialization, UserState) {
        UserState s_send{123, Directions::DOWN};
        Message m_send{1, s_send};
        Message m_recv{m_send.message, m_send.lenght};
        auto s_recv = m_recv.GetUserState();

        EXPECT_EQ(m_recv.lenght, m_send.lenght);
        EXPECT_EQ(m_recv.player, m_send.player);
        EXPECT_EQ(s_recv.direction, s_send.direction);
        EXPECT_EQ(s_recv.total_distance, s_send.total_distance);
    }

} // namespace