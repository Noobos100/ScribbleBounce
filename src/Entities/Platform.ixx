module;
#include <SFML/Graphics.hpp>

export module Platform;

export class Platform
{
    sf::RectangleShape shape;

public:
    Platform(float x, float y, float width, float height);

    void draw(sf::RenderWindow& window) const;
    sf::FloatRect GetBounds() const;
};
