module;
#include <SFML/Graphics.hpp>

module SfmlRenderer;

SfmlRenderer::SfmlRenderer(sf::RenderWindow& window)
    : window(window)
{
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        throw std::runtime_error("Unable to load the Windows Arial font.");
}

void SfmlRenderer::Clear(const Color color)
{
    window.clear({color.red, color.green, color.blue, color.alpha});
}

void SfmlRenderer::DrawRectangle(const Rectangle rectangle, const Color color)
{
    sf::RectangleShape shape;
    shape.setSize({rectangle.size.x, rectangle.size.y});
    shape.setPosition({
        rectangle.position.x - horizontalCameraOffset,
        rectangle.position.y + worldCameraOffset
    });
    shape.setFillColor({color.red, color.green, color.blue, color.alpha});
    window.draw(shape);
}

void SfmlRenderer::DrawCircle(
    const Vector2 center,
    const float radius,
    const Color color)
{
    sf::CircleShape shape(radius);
    shape.setOrigin({radius, radius});
    shape.setPosition({
        center.x - horizontalCameraOffset,
        center.y + worldCameraOffset
    });
    shape.setFillColor({color.red, color.green, color.blue, color.alpha});
    window.draw(shape);
}

void SfmlRenderer::DrawText(
    const std::string_view value,
    const Vector2 position,
    const unsigned int characterSize,
    const Color color)
{
    const auto originalView = window.getView();
    window.setView(window.getDefaultView());

    sf::Text text(font, std::string(value));
    text.setCharacterSize(characterSize);
    text.setFillColor({color.red, color.green, color.blue, color.alpha});

    const auto bounds = text.getLocalBounds();
    text.setPosition({
        position.x - bounds.position.x,
        position.y - bounds.position.y
    });
    window.draw(text);

    window.setView(originalView);
}

TextSize SfmlRenderer::MeasureText(
    const std::string_view value,
    const unsigned int characterSize)
{
    sf::Text text(font, std::string(value));
    text.setCharacterSize(characterSize);
    const auto bounds = text.getLocalBounds();
    return {bounds.size.x, bounds.size.y};
}

void SfmlRenderer::SetWorldCameraOffset(
    const float verticalOffset,
    const float horizontalOffset)
{
    worldCameraOffset = verticalOffset;
    horizontalCameraOffset = horizontalOffset;
}

void SfmlRenderer::Display()
{
    window.display();
}
