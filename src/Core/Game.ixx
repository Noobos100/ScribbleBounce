module;
#include <SFML/Graphics.hpp>

export module Game;

import std;
import Platform;
import Player;

/// Coordinates gameplay, platform collisions, and the upward-following camera.
export class Game
{
    std::vector<Platform> platforms;
    Player player;
    sf::Font heightFont;
    sf::Text heightText;
    /// Player's starting top edge in world coordinates, used as the height-score baseline.
    float initialPlayerY;
    /// Greatest upward distance reached from the starting position, in world units.
    float maximumHeight{};
    /// True after the player falls below the visible world area.
    bool isGameOver{};
    /// Window width in world units, used to keep generated platforms on screen.
    float windowWidth;
    /// Window height in SFML world-coordinate units, used to place the camera threshold.
    float windowHeight;
    /// Upward camera displacement in world-coordinate units; only increases during play.
    float cameraOffset{};
    /// Position of the latest reachable platform row used as the next generation anchor.
    float generationAnchorX;
    float generationAnchorWidth{120.f};
    float generationAnchorY{540.f};
    /// Random source used to vary generated platform positions and spacing.
    std::mt19937 randomEngine;

    /// Adds reachable platforms above the current highest platform.
    void GeneratePlatforms();
    /// Removes platforms that have passed below the camera's visible area.
    void RemovePlatformsBelowCamera();
    /// Restores the initial player, platform, camera, and score state.
    void ResetGame();

public:
    /**
     * Creates a game for the given window dimensions.
     *
     * The dimensions are floats to match SFML's floating-point world positions and view
     * coordinates. Although window pixel dimensions are usually integers, using floats
     * here avoids converting between coordinate types for player movement and camera math.
     *
     * @param windowWidth Window width in world-coordinate units; used for player bounds
     *                    and to keep generated platforms on screen.
     * @param windowHeight Window height in world-coordinate units; used to determine when
     *                     the camera should follow the player.
     */
    Game(float windowWidth, float windowHeight);

    /// Updates gameplay until the player falls below the visible area.
    /// @param deltaTime Elapsed time in seconds since the previous update.
    void Update(float deltaTime);

    /// Restarts the game if it has ended.
    void Restart();

    /// Draws the game through the camera view, with the score HUD and game-over display.
    /// @param window Render window whose view is temporarily adjusted for drawing.
    void Draw(sf::RenderWindow& window) const;
};
