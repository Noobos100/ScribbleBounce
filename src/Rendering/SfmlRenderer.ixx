module;
#include <SFML/Graphics.hpp>

export module SfmlRenderer;

import Renderer;

export class SfmlRenderer final : public Renderer
{
    sf::RenderWindow& window;
    sf::Font font;
    float worldCameraOffset{};
    float horizontalCameraOffset{};

public:
    explicit SfmlRenderer(sf::RenderWindow& window);

    void Clear(Color color) override;
    void DrawRectangle(Rectangle rectangle, Color color) override;
    void DrawCircle(Vector2 center, float radius, Color color) override;
    void DrawText(
        std::string_view text,
        Vector2 position,
        unsigned int characterSize,
        Color color) override;
    TextSize MeasureText(std::string_view text, unsigned int characterSize) override;
    void SetWorldCameraOffset(float verticalOffset, float horizontalOffset = 0.f) override;
    void Display() override;
};
