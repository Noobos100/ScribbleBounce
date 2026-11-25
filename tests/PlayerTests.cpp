#include <catch2/catch_test_macros.hpp>

import std;
import Player;

TEST_CASE("Player starts centered on the ground")
{
    const Player player{800.f, 580.f};

    CHECK(player.GetPosition().x == 380.f);
    CHECK(player.GetPosition().y == 530.f);
}

TEST_CASE("Player moves upward according to elapsed time")
{
    Player player{800.f, 300.f};
    const float initialY = player.GetPosition().y;

    player.update(0.1f);

    const float movedY = player.GetPosition().y;
    CHECK(movedY < initialY);
    CHECK(std::abs(movedY - 198.f) < 0.001f);
}

TEST_CASE("Player bounces automatically when reaching the ground")
{
    Player player{800.f, 300.f};

    player.update(1.f);
    CHECK(player.GetPosition().y == 250.f);

    player.update(0.1f);
    CHECK(player.GetPosition().y < 250.f);
}
