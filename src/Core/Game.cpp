module;
#include <SFML/Graphics.hpp>

module Game;

/**
 * @copydoc Game::Game(float, float)
 * @details The width is forwarded to the player, while the height is retained for camera
 *          tracking. Platform and player positions remain in world coordinates.
 */
Game::Game(const float windowWidth, const float windowHeight)
:
      platforms{{windowWidth / 2.f - 60.f, 540.f, 120.f, 16.f}},
      player(windowWidth, 540.f),
      heightText(heightFont),
      initialPlayerY(player.GetBounds().position.y),
      windowWidth(windowWidth),
      windowHeight(windowHeight),
      generationAnchorX(windowWidth / 2.f - 60.f),
      randomEngine(std::random_device{}())
{
    if (!heightFont.openFromFile("C:/Windows/Fonts/arial.ttf"))
        throw std::runtime_error("Unable to load the Windows Arial font.");

    heightText.setCharacterSize(24);
    heightText.setFillColor(sf::Color::Black);
    GeneratePlatforms();
}

/**
 * Adds rows of two or three platforms above the player's camera-following position.
 * One platform in each row is positioned within jumping distance of the previous row;
 * the other platforms are distributed across the window with space between them.
 */
void Game::GeneratePlatforms()
{
    // Génère des plateformes jusqu'à ce qu'une rangée dépasse le haut de la zone visible.
    const float generationLimit = windowHeight * 0.35f - cameraOffset - 120.f;

    while (generationAnchorY > generationLimit)
    {
        // Réduit graduellement la largeur des plateformes à mesure que la caméra monte.
        const float platformWidth = std::min(
            windowWidth,
            std::max(70.f, 95.f - cameraOffset * 0.015f)
        );
        // Choisit un écart vertical aléatoire pour la prochaine rangée.
        std::uniform_real_distribution gapDistribution(100.f, 130.f);
        const float platformGap = gapDistribution(randomEngine);
        const float platformY = generationAnchorY - platformGap;

        // Impose un décalage horizontal pour éviter une progression en restant immobile.
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
            const auto [minimumX, maximumX] = reachableRanges[rangeDistribution(randomEngine)];
            std::uniform_real_distribution xDistribution(minimumX, maximumX);
            pathPlatformX = xDistribution(randomEngine);
        }
        else
        {
            // Sur une fenêtre trop étroite, utilise la position accessible la plus éloignée.
            pathPlatformX = std::clamp(
                generationAnchorX + (generationAnchorX < maxX / 2.f ? 1.f : -1.f) * maximumHorizontalOffset,
                0.f,
                maxX);
        }

        platforms.emplace_back(
            pathPlatformX,
            platformY,
            platformWidth,
            16.f);

        // Garde cette plateforme comme point de départ pour la prochaine rangée.
        generationAnchorX = pathPlatformX;
        generationAnchorWidth = platformWidth;
        generationAnchorY = platformY;

        // Ajoute une plateforme facultative à la même hauteur.
        std::uniform_int_distribution<std::size_t> extraPlatformCountDistribution(0,1);
        const auto extraPlatformCount = extraPlatformCountDistribution(randomEngine);
        std::vector rowPlatformXs{pathPlatformX};
        std::uniform_real_distribution extraXDistribution(0.f, maxX);

        for (std::size_t index = 0; index < extraPlatformCount; ++index)
        {
            // Cherche une position laissant au moins 50 unités entre les plateformes de la rangée.
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
                platforms.emplace_back(*extraPlatformX, platformY, platformWidth, 16.f);
                rowPlatformXs.push_back(*extraPlatformX);
            }
        }
    }
}

/**
 * Removes platforms once their top edge is below the camera's lower visible boundary.
 */
void Game::RemovePlatformsBelowCamera()
{
    const float cameraBottom = windowHeight - cameraOffset;
    auto platform = platforms.begin();

    while (platform != platforms.end())
    {
        if (platform->GetBounds().position.y > cameraBottom)
            platform = platforms.erase(platform);
        else
            ++platform;
    }
}

/**
 * @copydoc Game::Update(float)
 * @details The camera offset is raised when the player's top reaches the upper 35 percent
 *          of the window. Keeping the largest offset prevents the view from moving down.
 */
void Game::Update(const float deltaTime)
{
    const auto previousBounds = player.GetBounds();
    player.update(deltaTime);
    const auto currentBounds = player.GetBounds();

    if (player.IsFalling())
    {
        const float previousBottom = previousBounds.position.y + previousBounds.size.y;
        const float currentBottom = currentBounds.position.y + currentBounds.size.y;

        // gestion des collisions avec les plateformes
        for (const auto& platform : platforms)
        {
            const auto platformBounds = platform.GetBounds();
            const float platformRight = platformBounds.position.x + platformBounds.size.x;
            const float playerRight = currentBounds.position.x + currentBounds.size.x;
            const bool overlapsHorizontally =
                currentBounds.position.x < platformRight &&
                playerRight > platformBounds.position.x;
            const bool crossesPlatformTop =
                previousBottom <= platformBounds.position.y &&
                currentBottom >= platformBounds.position.y;

            if (overlapsHorizontally && crossesPlatformTop)
            {
                player.LandOn(platformBounds.position.y);
                break;
            }
        }
    }

    if (const float desiredOffset = windowHeight * 0.35f - player.GetBounds().position.y; desiredOffset > cameraOffset)
        cameraOffset = desiredOffset;

    maximumHeight = std::max(
        maximumHeight,
        initialPlayerY - player.GetBounds().position.y
    );

    GeneratePlatforms();
    RemovePlatformsBelowCamera();
}

/**
 * @copydoc Game::Draw(sf::RenderWindow&) const
 * @details The camera offset is applied to a copy of the window's current view, which is
 *          restored after drawing so the camera change does not affect later rendering.
 */
void Game::Draw(sf::RenderWindow& window) const
{
    const auto originalView = window.getView();
    auto cameraView = originalView;
    auto center = cameraView.getCenter();
    center.y -= cameraOffset;
    cameraView.setCenter(center);
    window.setView(cameraView);

    for (const auto& platform : platforms)
        platform.draw(window);
    player.draw(window);

    window.setView(window.getDefaultView());
    auto scoreText = heightText;
    scoreText.setString(
        "Score : " + std::to_string(static_cast<int>(maximumHeight))
    );
    const auto textBounds = scoreText.getLocalBounds();
    const auto windowSize = window.getSize();
    scoreText.setPosition({
        static_cast<float>(windowSize.x) - textBounds.position.x - textBounds.size.x - 16.f,
        16.f - textBounds.position.y
    });
    window.draw(scoreText);

    window.setView(originalView);
}
