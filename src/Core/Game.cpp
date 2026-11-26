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
      platforms{{windowWidth / 2.f - 75.f, 540.f, 150.f, 16.f}},
      player(windowWidth, 540.f),
      windowWidth(windowWidth),
      windowHeight(windowHeight),
      generationAnchorX(windowWidth / 2.f - 75.f),
      randomEngine(std::random_device{}())
{
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
            std::max(100.f, 150.f - cameraOffset * 0.025f)
        );
        // Choisit un écart vertical aléatoire pour la prochaine rangée.
        std::uniform_real_distribution gapDistribution(100.f, 130.f);
        const float platformGap = gapDistribution(randomEngine);
        const float platformY = generationAnchorY - platformGap;

        // Place une plateforme de passage à portée de saut de la rangée précédente.
        const float maxX = std::max(0.f, windowWidth - platformWidth);
        const float minReachableX = std::max(0.f, generationAnchorX - 120.f);
        const float maxReachableX = std::min(maxX, generationAnchorX + 120.f);
        std::uniform_real_distribution pathXDistribution(
            minReachableX,
            std::max(minReachableX, maxReachableX));
        const float pathPlatformX = pathXDistribution(randomEngine);

        platforms.emplace_back(
            pathPlatformX,
            platformY,
            platformWidth,
            16.f);

        // Garde cette plateforme comme point de départ pour la prochaine rangée.
        generationAnchorX = pathPlatformX;
        generationAnchorY = platformY;

        // Ajoute une plateforme facultative à la même hauteur.
        std::uniform_int_distribution<std::size_t> extraPlatformCountDistribution(0,1);
        const auto extraPlatformCount = extraPlatformCountDistribution(randomEngine);
        std::vector rowPlatformXs{pathPlatformX};
        std::uniform_real_distribution extraXDistribution(0.f, maxX);

        for (std::size_t index = 0; index < extraPlatformCount; ++index)
        {
            // Essaie plusieurs positions pour éviter que les plateformes se chevauchent.
            float extraPlatformX = extraXDistribution(randomEngine);
            for (int attempt = 0; attempt < 20; ++attempt)
            {
                extraPlatformX = extraXDistribution(randomEngine);
                const bool hasSpace = std::ranges::all_of(
                    rowPlatformXs,
                    [extraPlatformX, platformWidth](const float existingX)
                    {
                        return std::abs(extraPlatformX - existingX) >= platformWidth + 12.f;
                    });
                if (hasSpace)
                    break;
            }

            // Ajoute la plateforme à la rangée et mémorise sa position horizontale.
            platforms.emplace_back(extraPlatformX, platformY, platformWidth, 16.f);
            rowPlatformXs.push_back(extraPlatformX);
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

    window.setView(originalView);
}
