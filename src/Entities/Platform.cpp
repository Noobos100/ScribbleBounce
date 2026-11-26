module;
#include <SFML/Graphics.hpp>

module Platform;

Platform::Platform(const float x, const float y, const float width, const float height)
{
    shape.setSize({width, height});
    shape.setPosition({x, y});
    shape.setFillColor(sf::Color(80, 160, 80));
}

void Platform::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::FloatRect Platform::GetBounds() const
{
    return shape.getGlobalBounds();
}
