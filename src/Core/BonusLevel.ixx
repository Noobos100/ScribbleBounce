export module BonusLevel;

import std;
import Platform;

/// Creates and updates the moving platforms used in the bonus level.
export class BonusLevel
{
    float windowWidth;
    std::mt19937 randomEngine;
    std::vector<Platform> platforms;

public:
    explicit BonusLevel(float windowWidth);

    /// Generates a fresh bonus route.
    void Start();

    /// Advances the bonus platforms.
    void Update(float deltaTime);

    /// Returns the horizontal player bounds for the bonus route.
    float GetWorldWidth() const;

    /// Returns the bonus route platforms.
    const std::vector<Platform>& GetPlatforms() const;
};
