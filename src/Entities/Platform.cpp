module;
#include <SFML/Graphics.hpp>

module Platform;

Platform::Platform(const float x, const float y, const float width, const float height)
    : bounds{{x, y}, {width, height}}
{
}

void Platform::Draw(Renderer& renderer) const
{
    renderer.DrawRectangle(bounds, {80, 160, 80});
}

Rectangle Platform::GetBounds() const
{
    return bounds;
}
