module;

module Door;

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

void Door::MoveHorizontally(const float distance)
{
    center.x += distance;
}
