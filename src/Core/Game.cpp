module;

module Game;

/**
 * @copydoc Game::Game(float, float)
 * @details The width is forwarded to the player, while the height is retained for camera
 *          tracking. Platform and player positions remain in world coordinates.
 */
Game::Game(const float windowWidth, const float windowHeight)
:
      levelGenerator(windowWidth, windowHeight),
      bonusLevel(windowWidth),
      player(windowWidth, 540.f),
      runStartPlayerY(player.GetBounds().position.y),
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
    runStartPlayerY = player.GetBounds().position.y;
    scoreAtRunStart = 0.f;
    maximumRiseThisRun = 0.f;
    maximumHeight = 0.f;
    cameraOffset = 0.f;
    horizontalCameraOffset = 0.f;
    savedNormalPlayer.reset();
    bonusLevelActive = false;
    bonusAnnouncementElapsed = 0.f;
    mustLeaveDoor = false;
    isGameOver = false;
    levelGenerator.Generate(cameraOffset, maximumHeight);
}

void Game::LandOnPlatforms(
    const Rectangle previousBounds,
    const Rectangle currentBounds,
    const std::vector<Platform>& platforms)
{
    if (!player.IsFalling())
        return;

    const float previousBottom = previousBounds.position.y + previousBounds.size.y;
    const float currentBottom = currentBounds.position.y + currentBounds.size.y;
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

bool Game::TouchesDoor(const Rectangle playerBounds) const
{
    return std::ranges::any_of(
        levelGenerator.GetPlatforms(),
        [playerBounds](const Platform& platform)
        {
            return platform.TouchesDoor(playerBounds);
        });
}

void Game::EnterBonusLevel()
{
    savedNormalPlayer = player;
    bonusLevel.Start();
    bonusLevelActive = true;
    bonusAnnouncementElapsed = 0.f;
    player.SetHorizontalMovementBounds(0.f, bonusLevel.GetWorldWidth());
    const auto startPlatform = bonusLevel.GetPlatforms().front().GetBounds();
    player.RespawnOnPlatform(
        startPlatform.position.x + startPlatform.size.x / 2.f,
        startPlatform.position.y);
    horizontalCameraOffset = 0.f;
    mustLeaveDoor = false;
}

void Game::ExitBonusLevel()
{
    bonusLevelActive = false;
    if (savedNormalPlayer)
        player = std::move(*savedNormalPlayer);
    savedNormalPlayer.reset();
    horizontalCameraOffset = 0.f;
    mustLeaveDoor = true;
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

    if (bonusLevelActive)
    {
        bonusAnnouncementElapsed += deltaTime;
        LandOnPlatforms(previousBounds, currentBounds, bonusLevel.GetPlatforms());
        bonusLevel.Update(deltaTime);

        if (player.GetBounds().position.y > windowHeight)
        {
            ExitBonusLevel();
            return;
        }

        horizontalCameraOffset = std::max(
            horizontalCameraOffset,
            player.GetBounds().position.x - windowWidth * 0.6f);
        return;
    }

    LandOnPlatforms(previousBounds, currentBounds, levelGenerator.GetPlatforms());

    if (mustLeaveDoor && !TouchesDoor(player.GetBounds()))
        mustLeaveDoor = false;

    if (const float desiredOffset = windowHeight * 0.35f - player.GetBounds().position.y; desiredOffset > cameraOffset)
        cameraOffset = desiredOffset;

    maximumRiseThisRun = std::max(
        maximumRiseThisRun,
        runStartPlayerY - player.GetBounds().position.y);
    maximumHeight = scoreAtRunStart + maximumRiseThisRun;

    if (!mustLeaveDoor && levelGenerator.RemoveTouchedDoor(player.GetBounds()))
    {
        EnterBonusLevel();
        return;
    }

    // à partir d'une certaine hauteur aléatoire, on fait apparaître des plateformes mobiles
    if (maximumHeight >= std::clamp(maximumHeight, 1500.f, 2000.f))
    {
        levelGenerator.EnableMovingPlatforms();
    }
    levelGenerator.Update(deltaTime);

    // si le joueur tombe en dehors de la caméra, c'est perdu
    if (player.GetBounds().position.y > windowHeight - cameraOffset)
    {
        isGameOver = true;
        return;
    }

    levelGenerator.Generate(cameraOffset, maximumHeight);
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
 * @copydoc Game::Continue()
 */
void Game::Continue()
{
    if (isGameOver && retryCount > 0)
    {
        const Platform* lowestStationaryPlatform = nullptr;
        const Platform* lowestPlatform = nullptr;
        for (const auto& platform : levelGenerator.GetPlatforms())
        {
            if (lowestPlatform == nullptr ||
                platform.GetBounds().position.y > lowestPlatform->GetBounds().position.y)
            {
                lowestPlatform = &platform;
            }

            if (!platform.IsMoving() &&
                (lowestStationaryPlatform == nullptr ||
                 platform.GetBounds().position.y >
                     lowestStationaryPlatform->GetBounds().position.y))
            {
                lowestStationaryPlatform = &platform;
            }
        }

        const Platform* respawnPlatform =
            lowestStationaryPlatform != nullptr ? lowestStationaryPlatform : lowestPlatform;
        if (respawnPlatform == nullptr)
            return;

        const auto platformBounds = respawnPlatform->GetBounds();
        --retryCount;
        player.RespawnOnPlatform(
            platformBounds.position.x + platformBounds.size.x / 2.f,
            platformBounds.position.y);
        runStartPlayerY = player.GetBounds().position.y;
        scoreAtRunStart = maximumHeight;
        maximumRiseThisRun = 0.f;
        isGameOver = false;
    }
}

/**
 * @copydoc Game::GrantRetry()
 */
void Game::GrantRetry()
{
    ++retryCount;
}

Color Game::GetBackgroundColor() const
{
    return bonusLevelActive
        ? Color{170, 75, 75}
        : Color{135, 206, 235};
}

/**
 * @copydoc Game::Draw(Renderer&) const
 */
void Game::Draw(Renderer& renderer) const
{
    renderer.SetWorldCameraOffset(
        bonusLevelActive ? 0.f : cameraOffset,
        horizontalCameraOffset);

    const auto& platforms = bonusLevelActive
        ? bonusLevel.GetPlatforms()
        : levelGenerator.GetPlatforms();
    for (const auto& platform : platforms)
        platform.Draw(renderer);
    player.Draw(renderer);

    renderer.SetWorldCameraOffset(0.f, 0.f);
    const auto scoreText = "Score : " + std::to_string(static_cast<int>(maximumHeight));
    const auto [scoreWidth, scoreHeight] = renderer.MeasureText(scoreText, 24);
    renderer.DrawText(
        scoreText,
        {windowWidth - scoreWidth - 16.f, 16.f},
        24,
        {0, 0, 0});

    constexpr float bonusAnnouncementDuration = 3.f;
    constexpr float bonusFlashInterval = 0.25f;
    if (bonusLevelActive && bonusAnnouncementElapsed < bonusAnnouncementDuration &&
        static_cast<unsigned int>(bonusAnnouncementElapsed / bonusFlashInterval) % 2 == 0)
    {
        constexpr std::string_view bonusText = "BONUS";
        constexpr unsigned int bonusTextSize = 48;
        const auto [bonusTextWidth, bonusTextHeight] =
            renderer.MeasureText(bonusText, bonusTextSize);
        renderer.DrawText(
            bonusText,
            {(windowWidth - bonusTextWidth) / 2.f, (windowHeight - bonusTextHeight) / 2.f},
            bonusTextSize,
            {255, 230, 80});
    }

    const auto retryText = "Retry: " + std::to_string(retryCount);
    renderer.DrawText(retryText, {16.f, 16.f}, 24, {0, 0, 0});

    if (isGameOver)
    {
        const auto gameOverText =
            "Game Over\nFinal Score: " +
            std::to_string(static_cast<int>(maximumHeight)) +
            (retryCount > 0
                ? "\nPress C to Continue\nPress R to Restart"
                : "\nPress R to Restart");
        const auto [gameOverWidth, gameOverHeight] = renderer.MeasureText(gameOverText, 32);
        renderer.DrawText(
            gameOverText,
            {(windowWidth - gameOverWidth) / 2.f, (windowHeight - gameOverHeight) / 2.f},
            32,
            {0, 0, 0});
    }
}
