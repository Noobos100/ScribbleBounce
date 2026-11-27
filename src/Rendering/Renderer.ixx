export module Renderer;

import std;

export struct Vector2
{
    float x{};
    float y{};
};

export struct Rectangle
{
    Vector2 position;
    Vector2 size;
};

export struct Color
{
    std::uint8_t red{};
    std::uint8_t green{};
    std::uint8_t blue{};
    std::uint8_t alpha{255};
};

export struct TextSize
{
    float width{};
    float height{};
};

/// Backend-independent drawing interface for the game.
export class Renderer
{
public:
    virtual ~Renderer() = default;

    /// Clears the current frame to the requested color.
    virtual void Clear(Color color) = 0;
    /// Draws a world-space rectangle with the currently configured camera offset.
    virtual void DrawRectangle(Rectangle rectangle, Color color) = 0;
    /// Draws a world-space circle with the currently configured camera offset.
    virtual void DrawCircle(Vector2 center, float radius, Color color) = 0;
    /// Draws text at a screen-space top-left position.
    virtual void DrawText(
        std::string_view text,
        Vector2 position,
        unsigned int characterSize,
        Color color) = 0;
    /// Returns the rendered text's width and height in screen coordinates.
    virtual TextSize MeasureText(std::string_view text, unsigned int characterSize) = 0;
    /// Sets the vertical world-to-screen offset used for world-space rectangles.
    virtual void SetWorldCameraOffset(float offset) = 0;
    /// Presents the completed frame.
    virtual void Display() = 0;
};
