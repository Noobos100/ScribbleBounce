module;
export module Platform;

import Renderer;

export class Platform
{
    Rectangle bounds;
    bool horizontalMovementEligible;
    float horizontalVelocity{};
    float minimumX{};
    float maximumX{};

public:
    Platform(float x, float y, float width, float height, bool horizontalMovementEligible = false);

    void Draw(Renderer& renderer) const;
    Rectangle GetBounds() const;
    bool IsHorizontalMovementEligible() const;
    void StartHorizontalMovement(float minimumX, float maximumX, float speed);
    void Update(float deltaTime);
};
