module;
#include <SFML/Graphics.hpp>
export module Player;

export class Player
{
    sf::RectangleShape shape;
    float speed;
    float windowWidth;
    float verticalVelocity;

    public:
        Player(float windowWidth, float startPlatformTop);

        void update(float deltaTime);
        void draw(sf::RenderWindow& window) const;
        sf::Vector2f GetPosition() const;
        sf::FloatRect GetBounds() const;
        bool IsFalling() const;
        void LandOn(float platformTop);
};
