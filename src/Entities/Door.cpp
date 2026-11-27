module;

module Door;

import std;

Door::Door(const Vector2 center, const float radius)
    : center(center),
      radius(radius)
{
}

void Door::Draw(Renderer& renderer) const
{
    renderer.DrawCircle(center, radius, {0, 0, 0});
    renderer.DrawRectangle(
        {{center.x - radius, center.y}, {radius * 2.f, radius}},
        {0, 0, 0});
}

bool Door::Intersects(const Rectangle rectangle) const
{
    const float closestX = std::clamp(
        center.x,
        rectangle.position.x,
        rectangle.position.x + rectangle.size.x);
    const float closestY = std::clamp(
        center.y,
        rectangle.position.y,
        rectangle.position.y + rectangle.size.y);
    const float distanceX = center.x - closestX;
    const float distanceY = center.y - closestY;
    return distanceX * distanceX + distanceY * distanceY <= radius * radius;
}

void Door::MoveHorizontally(const float distance)
{
    center.x += distance;
}
