#include <catch2/catch_test_macros.hpp>

import std;
import Player;

TEST_CASE("Player starts centered on the ground")
{
    const Player player{800.f, 580.f};

    CHECK(player.GetPosition().x == 380.f);
    CHECK(player.GetPosition().y == 530.f);
}

TEST_CASE("Player moves upward after landing according to elapsed time")
{
    Player player{800.f, 300.f};
    player.LandOn(300.f);

    player.update(0.1f);

    CHECK(std::abs(player.GetPosition().y - 178.f) < 0.001f);
}

TEST_CASE("Player bounces upward after landing")
{
    Player player{800.f, 300.f};

    player.update(1.f);
    CHECK(player.GetPosition().y == 2050.f);
    CHECK(player.IsFalling());

    player.LandOn(300.f);
    CHECK(player.GetPosition().y == 250.f);
    CHECK_FALSE(player.IsFalling());

    player.update(0.1f);
    CHECK(player.GetPosition().y < 250.f);
}
