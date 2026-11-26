module;
#include <SFML/Graphics.hpp>

module Game;

Game::Game(const float windowWidth)
    : platforms{
          {300.f, 540.f, 200.f, 16.f},
          {180.f, 440.f, 150.f, 16.f},
          {390.f, 345.f, 150.f, 16.f},
          {200.f, 250.f, 160.f, 16.f},
          {400.f, 155.f, 150.f, 16.f}
      },
      player(windowWidth, 540.f)
{
}

void Game::Update(const float deltaTime)
{
    const auto previousBounds = player.GetBounds();
    player.update(deltaTime);
    const auto currentBounds = player.GetBounds();

    if (!player.IsFalling())
        return;

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

void Game::Draw(sf::RenderWindow& window) const
{
    for (const auto& platform : platforms)
        platform.draw(window);
    player.draw(window);
}
