module;
export module Player;

import Renderer;

export class Player
{
    Vector2 position;
    Vector2 size{40.f, 50.f};
    float speed;
    float windowWidth;
    float verticalVelocity;

    public:
        Player(float windowWidth, float startPlatformTop);

        void update(float deltaTime);
        void Draw(Renderer& renderer) const;
        Vector2 GetPosition() const;
        Rectangle GetBounds() const;
        bool IsFalling() const;
        void LandOn(float platformTop);
};
