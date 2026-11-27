export module Door;

import Renderer;

/// A decorative black circular doorway attached to a platform.
export class Door
{
    Vector2 center;
    float radius;

public:
    Door(Vector2 center, float radius);

    /// Draws the doorway in world coordinates.
    void Draw(Renderer& renderer) const;

    /// Returns whether a world-space rectangle overlaps the doorway.
    bool Intersects(Rectangle rectangle) const;

    /// Keeps the doorway aligned when its platform moves horizontally.
    void MoveHorizontally(float distance);
};
