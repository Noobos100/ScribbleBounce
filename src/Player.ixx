module;
#include <SFML/Graphics.hpp>
export module Player;

export class Player
{
    sf::RectangleShape shape;
    float speed;
    float windowWidth;
    float groundLevel;
    float verticalVelocity;

    public:
        Player(float windowWidth, float groundLevel);

        void update(float deltaTime);
        void draw(sf::RenderWindow& window) const;
        sf::Vector2f GetPosition() const;
};
