#include <catch2/catch_test_macros.hpp>

import Platform;

TEST_CASE("Platform preserves its bounds")
{
    const Platform platform{20.f, 30.f, 100.f, 16.f};
    const auto bounds = platform.GetBounds();

    CHECK(bounds.position.x == 20.f);
    CHECK(bounds.position.y == 30.f);
    CHECK(bounds.size.x == 100.f);
    CHECK(bounds.size.y == 16.f);
    CHECK_FALSE(platform.IsHorizontalMovementEligible());
}

TEST_CASE("Ineligible platforms remain stationary")
{
    Platform platform{50.f, 30.f, 100.f, 16.f};
    platform.StartHorizontalMovement(0.f, 100.f, 20.f);

    platform.Update(1.f);

    CHECK(platform.GetBounds().position.x == 50.f);
}

TEST_CASE("Eligible platforms move and reverse at their horizontal limits")
{
    Platform platform{50.f, 30.f, 100.f, 16.f, true};
    platform.StartHorizontalMovement(10.f, 90.f, 20.f);

    platform.Update(1.f);
    CHECK(platform.GetBounds().position.x == 70.f);

    platform.Update(2.f);
    CHECK(platform.GetBounds().position.x == 70.f);
}

TEST_CASE("Eligible platform rejects invalid movement bounds")
{
    Platform platform{50.f, 30.f, 100.f, 16.f, true};
    platform.StartHorizontalMovement(50.f, 50.f, 20.f);

    platform.Update(1.f);

    CHECK(platform.GetBounds().position.x == 50.f);
}
