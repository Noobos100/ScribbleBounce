module;
export module Player;

import Renderer;

export class Player
{
    Vector2 position;
    Vector2 size{40.f, 50.f};
    float speed;
    float minimumX;
    float maximumX;
    float verticalVelocity;

    public:
        Player(float windowWidth, float startPlatformTop);

        void update(float deltaTime);
        void Draw(Renderer& renderer) const;
        Vector2 GetPosition() const;
        Rectangle GetBounds() const;
        void SetHorizontalMovementBounds(float minimumX, float maximumX);
        bool IsFalling() const;
        void LandOn(float platformTop);
        void RespawnOnPlatform(float platformCenterX, float platformTop);
};
