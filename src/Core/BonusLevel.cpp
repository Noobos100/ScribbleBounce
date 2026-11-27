module BonusLevel;

import std;

BonusLevel::BonusLevel(const float windowWidth)
    : windowWidth(windowWidth),
      randomEngine(std::random_device{}())
{
}

void BonusLevel::Start()
{
    constexpr float platformWidth = 64.f;
    constexpr float platformHeight = 16.f;
    constexpr float minimumPlatformSpacing = 150.f;
    constexpr float maximumPlatformSpacing = 200.f;

    platforms.clear();
    platforms.emplace_back(38.f, 540.f, platformWidth, platformHeight);
    std::uniform_int_distribution platformCountDistribution(10, 15 );
    std::uniform_real_distribution spacingDistribution(
        minimumPlatformSpacing,
        maximumPlatformSpacing
    );
    std::uniform_real_distribution heightChangeDistribution(-80.f, 100.f);
    std::uniform_int_distribution directionDistribution(0, 1);
    float platformX = platforms.front().GetBounds().position.x;
    float platformTop = platforms.front().GetBounds().position.y;
    const int movingPlatformCount = platformCountDistribution(randomEngine);
    for (int index = 0; index < movingPlatformCount; ++index)
    {
        constexpr float maximumPlatformTop = 520.f;
        constexpr float minimumPlatformTop = 340.f;
        constexpr float platformMovementRange = 18.f;
        constexpr float platformMovementSpeed = 50.f;
        platformX += spacingDistribution(randomEngine);
        platformTop = std::clamp(
            platformTop + heightChangeDistribution(randomEngine),
            minimumPlatformTop,
            maximumPlatformTop);
        platforms.emplace_back(platformX, platformTop, platformWidth, platformHeight, true);

        const float minimumX = std::max(0.f, platformX - platformMovementRange);
        const float maximumX = platformX + platformMovementRange;
        const float direction = directionDistribution(randomEngine) == 0 ? -1.f : 1.f;
        platforms.back().StartHorizontalMovement(
            minimumX,
            maximumX,
            direction * platformMovementSpeed);
    }
}

void BonusLevel::Update(const float deltaTime)
{
    for (auto& platform : platforms)
        platform.Update(deltaTime);
}

float BonusLevel::GetWorldWidth() const
{
    if (platforms.empty())
        return windowWidth;

    const auto [position, size] = platforms.back().GetBounds();
    return position.x + size.x + windowWidth;
}

const std::vector<Platform>& BonusLevel::GetPlatforms() const
{
    return platforms;
}
