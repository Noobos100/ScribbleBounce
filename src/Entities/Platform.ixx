module;
export module Platform;

import std;
import Renderer;
import Door;

export class Platform
{
    Rectangle bounds;
    bool horizontalMovementEligible;
    std::optional<Door> door;
    float horizontalVelocity{};
    float minimumX{};
    float maximumX{};

public:
    Platform(float x, float y, float width, float height, bool horizontalMovementEligible = false);

    void Draw(Renderer& renderer) const;
    Rectangle GetBounds() const;
    bool IsHorizontalMovementEligible() const;
    bool IsMoving() const;
    void AddDoor();
    void StartHorizontalMovement(float minimumX, float maximumX, float speed);
    void Update(float deltaTime);
};
