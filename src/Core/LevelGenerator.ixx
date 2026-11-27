export module LevelGenerator;

import std;
import Platform;

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

public:
    LevelGenerator(float windowWidth, float windowHeight);

    /// Restores the starting platform and clears previously generated platforms.
    void Reset();

    /// Generates reachable platform rows above the camera's generation limit.
    void Generate(float cameraOffset);

    /// Removes platforms below the bottom edge of the visible world.
    void RemoveBelowCamera(float cameraOffset);

    /// Returns the platform collection for collision detection and rendering.
    const std::vector<Platform>& GetPlatforms() const;
};
