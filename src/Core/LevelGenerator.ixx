export module LevelGenerator;

import std;
import Platform;
import Renderer;

/// Generates and maintains the platforms that make up one game level.
export class LevelGenerator
{
    float windowWidth;
    float windowHeight;
    float generationAnchorX;
    float generationAnchorWidth;
    float generationAnchorY;
    std::mt19937 randomEngine;
    std::vector<Platform> platforms;
    bool movingPlatformsEnabled{};

    void StartMovingPlatform(Platform& platform);

public:
    LevelGenerator(float windowWidth, float windowHeight);

    /// Restores the starting platform and clears previously generated platforms.
    void Reset();

    /// Removes the door touched by the given player bounds and reports whether one was found.
    bool RemoveTouchedDoor(Rectangle playerBounds);

    /// Generates reachable platform rows above the camera's generation limit.
    void Generate(float cameraOffset, float currentHeight);

    /// Enables horizontal movement for a subset of optional platforms.
    void EnableMovingPlatforms();

    /// Advances any moving platforms by the elapsed time.
    void Update(float deltaTime);

    /// Removes platforms below the bottom edge of the visible world.
    void RemoveBelowCamera(float cameraOffset);

    /// Returns the platform collection for collision detection and rendering.
    const std::vector<Platform>& GetPlatforms() const;
};
