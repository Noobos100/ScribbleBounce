module;
export module Game;

import std;
import Renderer;
import Platform;
import Player;
import LevelGenerator;
import BonusLevel;

/// Coordinates gameplay, platform collisions, and the upward-following camera.
export class Game
{
    LevelGenerator levelGenerator;
    BonusLevel bonusLevel;
    Player player;
    /// Player's top edge when the current scoring run segment began.
    float runStartPlayerY;
    /// Score already accumulated before the current scoring run segment.
    float scoreAtRunStart{};
    /// Greatest upward distance reached within the current scoring run segment.
    float maximumRiseThisRun{};
    /// Greatest upward distance reached from the starting position, in world units.
    float maximumHeight{};
    /// Number of continues available after falling.
    unsigned int retryCount{};
    /// True after the player falls below the visible world area.
    bool isGameOver{};
    /// Window width in world units, used to keep generated platforms on screen.
    float windowWidth;
    /// Window height in SFML world-coordinate units, used to place the camera threshold.
    float windowHeight;
    /// Upward camera displacement in world-coordinate units; only increases during play.
    float cameraOffset{};
    /// Rightward camera displacement used only while scrolling through the bonus level.
    float horizontalCameraOffset{};
    std::optional<Player> savedNormalPlayer;
    bool bonusLevelActive{};
    float bonusAnnouncementElapsed{};
    /// Prevents immediately re-entering the same door after returning to the normal level.
    bool mustLeaveDoor{};
    /// Resets the player, platforms, camera, and score for a new run.
    void ResetGame();
    void LandOnPlatforms(
        Rectangle previousBounds,
        Rectangle currentBounds,
        const std::vector<Platform>& platforms);
    bool TouchesDoor(Rectangle playerBounds) const;
    void EnterBonusLevel();
    void ExitBonusLevel();

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

    /// Starts a new run with a reset score after game over.
    void Restart();

    /// Continues from the current platforms while preserving the score, if a retry is available.
    void Continue();

    /// Adds one available continue.
    void GrantRetry();

    /// Returns the background color for the currently active level.
    Color GetBackgroundColor() const;

    /// Draws the game and its screen-space UI through the renderer abstraction.
    void Draw(Renderer& renderer) const;
};
