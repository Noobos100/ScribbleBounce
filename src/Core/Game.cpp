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
      levelGenerator(windowWidth, windowHeight),
      player(windowWidth, 540.f),
      initialPlayerY(player.GetBounds().position.y),
      windowWidth(windowWidth),
      windowHeight(windowHeight)
{
    ResetGame();
}

/**
 * @copydoc Game::ResetGame()
 */
void Game::ResetGame()
{
    constexpr float startPlatformTop = 540.f;

    levelGenerator.Reset();
    player = Player(windowWidth, startPlatformTop);
    initialPlayerY = player.GetBounds().position.y;
    maximumHeight = 0.f;
    cameraOffset = 0.f;
    isGameOver = false;
    levelGenerator.Generate(cameraOffset);
}

/**
 * @copydoc Game::Update(float)
 * @details The camera offset is raised when the player's top reaches the upper 35 percent
 *          of the window. Keeping the largest offset prevents the view from moving down.
 */
void Game::Update(const float deltaTime)
{
    if (isGameOver)
        return;

    const auto previousBounds = player.GetBounds();
    player.update(deltaTime);
    const auto currentBounds = player.GetBounds();

    if (player.IsFalling())
    {
        const float previousBottom = previousBounds.position.y + previousBounds.size.y;
        const float currentBottom = currentBounds.position.y + currentBounds.size.y;

        // gestion des collisions avec les plateformes
        for (const auto& platform : levelGenerator.GetPlatforms())
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

    if (player.GetBounds().position.y > windowHeight - cameraOffset)
    {
        isGameOver = true;
        return;
    }

    levelGenerator.Generate(cameraOffset);
    levelGenerator.RemoveBelowCamera(cameraOffset);
}

/**
 * @copydoc Game::Restart()
 */
void Game::Restart()
{
    if (isGameOver)
        ResetGame();
}

/**
 * @copydoc Game::Draw(Renderer&) const
 */
void Game::Draw(Renderer& renderer) const
{
    renderer.SetWorldCameraOffset(cameraOffset);

    for (const auto& platform : levelGenerator.GetPlatforms())
        platform.Draw(renderer);
    player.Draw(renderer);

    renderer.SetWorldCameraOffset(0.f);
    const auto scoreText = "Score : " + std::to_string(static_cast<int>(maximumHeight));
    const auto [width, height] = renderer.MeasureText(scoreText, 24);
    renderer.DrawText(
        scoreText,
        {windowWidth - width - 16.f, 16.f},
        24,
        {0, 0, 0});

    if (isGameOver)
    {
        const auto gameOverText =
            "Game Over\nFinal Score: " +
            std::to_string(static_cast<int>(maximumHeight)) +
            "\nPress R to Restart";
        const auto [width, height] = renderer.MeasureText(gameOverText, 32);
        renderer.DrawText(
            gameOverText,
            {(windowWidth - width) / 2.f, (windowHeight - height) / 2.f},
            32,
            {0, 0, 0});
    }
}
