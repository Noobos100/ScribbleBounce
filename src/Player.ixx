module;
#include <SFML/Graphics.hpp>
export module Player;

export class Player
{
    sf::RectangleShape shape;
    float speed;
    float windowWidth;
    float windowHeight;

    public:
        Player(float windowWidth, float windowHeight);

        void update(float deltaTime);
        void draw(sf::RenderWindow& window) const;
};
