module LevelGenerator;

import std;

LevelGenerator::LevelGenerator(const float windowWidth, const float windowHeight)
    : windowWidth(windowWidth),
      windowHeight(windowHeight),
      generationAnchorX(windowWidth / 2.f - 60.f),
      generationAnchorWidth(120.f),
      generationAnchorY(540.f),
      randomEngine(std::random_device{}())
{
    Reset();
}

void LevelGenerator::Reset()
{
    constexpr float startPlatformTop = 540.f;
    constexpr float startPlatformWidth = 120.f;
    const float startPlatformX = windowWidth / 2.f - startPlatformWidth / 2.f;

    platforms.clear();
    platforms.emplace_back(startPlatformX, startPlatformTop, startPlatformWidth, 16.f);
    generationAnchorX = startPlatformX;
    generationAnchorWidth = startPlatformWidth;
    generationAnchorY = startPlatformTop;
    movingPlatformsEnabled = false;
}

void LevelGenerator::StartMovingPlatform(Platform& platform)
{
    constexpr float movementRange = 80.f;
    constexpr float movementSpeed = 70.f;
    const float platformX = platform.GetBounds().position.x;
    const float platformWidth = platform.GetBounds().size.x;
    const float minimumX = std::max(0.f, platformX - movementRange);
    const float maximumX = std::min(
        windowWidth - platformWidth,
        platformX + movementRange);
    std::uniform_int_distribution directionDistribution(0, 1);
    const float direction = directionDistribution(randomEngine) == 0 ? -1.f : 1.f;
    platform.StartHorizontalMovement(minimumX, maximumX, direction * movementSpeed);
}

void LevelGenerator::Generate(const float cameraOffset, const float currentHeight)
{
    const float generationLimit = windowHeight * 0.35f - cameraOffset - 120.f;
    std::bernoulli_distribution shouldMove(0.5);

    while (generationAnchorY > generationLimit)
    {
        const float platformWidth = std::min(
            windowWidth,
            std::max(70.f, 95.f - cameraOffset * 0.015f));

        std::uniform_real_distribution gapDistribution(100.f, 150.f);
        const float platformGap = gapDistribution(randomEngine);
        const float platformY = generationAnchorY - platformGap;

        const float maxX = std::max(0.f, windowWidth - platformWidth);
        constexpr float maximumHorizontalOffset = 220.f;
        constexpr float clearanceForStandingPlayer = 60.f;
        std::vector<std::pair<float, float>> reachableRanges;

        const float rightMinX = std::max(
            0.f,
            generationAnchorX + generationAnchorWidth + clearanceForStandingPlayer);
        const float rightMaxX = std::min(maxX, generationAnchorX + maximumHorizontalOffset);
        if (rightMinX <= rightMaxX)
            reachableRanges.emplace_back(rightMinX, rightMaxX);

        const float leftMinX = std::max(0.f, generationAnchorX - maximumHorizontalOffset);
        const float leftMaxX = std::min(
            maxX,
            generationAnchorX - platformWidth - clearanceForStandingPlayer);
        if (leftMinX <= leftMaxX)
            reachableRanges.emplace_back(leftMinX, leftMaxX);

        float pathPlatformX{};
        if (!reachableRanges.empty())
        {
            std::uniform_int_distribution<std::size_t> rangeDistribution(
                0,
                reachableRanges.size() - 1);
            const auto [minimumX, maximumX] =
                reachableRanges[rangeDistribution(randomEngine)];
            std::uniform_real_distribution xDistribution(minimumX, maximumX);
            pathPlatformX = xDistribution(randomEngine);
        }
        else
        {
            pathPlatformX = std::clamp(
                generationAnchorX +
                    (generationAnchorX < maxX / 2.f ? 1.f : -1.f) *
                        maximumHorizontalOffset,
                0.f,
                maxX);
        }

        platforms.emplace_back(pathPlatformX, platformY, platformWidth, 16.f, true);
        if (movingPlatformsEnabled && shouldMove(randomEngine))
            StartMovingPlatform(platforms.back());

        generationAnchorX = pathPlatformX;
        generationAnchorWidth = platformWidth;
        generationAnchorY = platformY;

        std::uniform_int_distribution<std::size_t> extraPlatformCountDistribution(0, 1);
        const auto extraPlatformCount = extraPlatformCountDistribution(randomEngine);
        std::vector rowPlatformXs{pathPlatformX};
        std::uniform_real_distribution extraXDistribution(0.f, maxX);

        for (std::size_t index = 0; index < extraPlatformCount; ++index)
        {
            std::optional<float> extraPlatformX;
            for (int attempt = 0; attempt < 64; ++attempt)
            {
                const float candidateX = extraXDistribution(randomEngine);
                const bool hasSpace = std::ranges::all_of(
                    rowPlatformXs,
                    [candidateX, platformWidth](const float existingX)
                    {
                        return std::abs(candidateX - existingX) >= platformWidth + 50.f;
                    });
                if (hasSpace)
                {
                    extraPlatformX = candidateX;
                    break;
                }
            }

            if (extraPlatformX)
            {
                platforms.emplace_back(*extraPlatformX, platformY, platformWidth, 16.f, true);
                if (movingPlatformsEnabled && shouldMove(randomEngine))
                    StartMovingPlatform(platforms.back());
                rowPlatformXs.push_back(*extraPlatformX);

                // à partir d'une certaine hauteur, 1 chance sur 10 (0,1) que des portes
                // vers des niveaux bonus peuvent apparaître sur les plateformes secondaires
                if (currentHeight > 5000.f)
                {
                    std::bernoulli_distribution shouldSpawnDoor(0.1);
                    if (shouldSpawnDoor(randomEngine))
                    {
                        std::uniform_int_distribution platformDistribution(
                            platforms.size() - 2,
                            platforms.size() - 1
                        );
                        platforms[platformDistribution(randomEngine)].AddDoor();
                    }
                }
            }
        }
    }
}

void LevelGenerator::EnableMovingPlatforms()
{
    movingPlatformsEnabled = true;
}

void LevelGenerator::Update(const float deltaTime)
{
    for (auto& platform : platforms)
        platform.Update(deltaTime);
}

void LevelGenerator::RemoveBelowCamera(const float cameraOffset)
{
    const float cameraBottom = windowHeight - cameraOffset;
    std::erase_if(
        platforms,
        [cameraBottom](const Platform& platform)
        {
            return platform.GetBounds().position.y > cameraBottom;
        });
}

const std::vector<Platform>& LevelGenerator::GetPlatforms() const
{
    return platforms;
}
