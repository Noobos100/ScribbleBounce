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
    /// Window width in world units, used to keep generated platforms on screen.
    float windowWidth;
    /// Window height in SFML world-coordinate units, used to place the camera threshold.
    float windowHeight;
    /// Upward camera displacement in world-coordinate units; only increases during play.
    float cameraOffset{};
    /// Position of the latest reachable platform row used as the next generation anchor.
    float generationAnchorX;
    float generationAnchorY{540.f};
    /// Random source used to vary generated platform positions and spacing.
    std::mt19937 randomEngine;

    /// Adds reachable platforms above the current highest platform.
    void GeneratePlatforms();
    /// Removes platforms that have passed below the camera's visible area.
    void RemovePlatformsBelowCamera();

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

    /// Updates the player, platform collisions, and upward-only camera tracking.
    /// @param deltaTime Elapsed time in seconds since the previous update.
    void Update(float deltaTime);

    /// Draws the platforms and player through the camera view.
    /// @param window Render window whose view is temporarily adjusted for drawing.
    void Draw(sf::RenderWindow& window) const;
};
