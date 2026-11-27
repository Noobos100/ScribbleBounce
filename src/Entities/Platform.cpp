module;
module Platform;

import std;
import Door;

Platform::Platform(
    const float x,
    const float y,
    const float width,
    const float height,
    const bool horizontalMovementEligible)
    : bounds{{x, y}, {width, height}},
      horizontalMovementEligible(horizontalMovementEligible)
{
}

void Platform::Draw(Renderer& renderer) const
{
    renderer.DrawRectangle(bounds, {80, 160, 80});
    if (door)
        door->Draw(renderer);
}

Rectangle Platform::GetBounds() const
{
    return bounds;
}

bool Platform::IsHorizontalMovementEligible() const
{
    return horizontalMovementEligible;
}

bool Platform::IsMoving() const
{
    return horizontalVelocity != 0.f;
}

void Platform::AddDoor()
{
    constexpr float doorRadius = 12.f;
    door.emplace(
        Vector2{
            .x = bounds.position.x + bounds.size.x / 2.f,
            .y = bounds.position.y - doorRadius
        },
        doorRadius);
}

void Platform::RemoveDoor()
{
    door.reset();
}

bool Platform::TouchesDoor(const Rectangle rectangle) const
{
    return door && door->Intersects(rectangle);
}

void Platform::StartHorizontalMovement(
    const float minimumX,
    const float maximumX,
    const float speed)
{
    if (!horizontalMovementEligible || minimumX >= maximumX || speed == 0.f)
        return;

    this->minimumX = minimumX;
    this->maximumX = maximumX;
    horizontalVelocity = speed;
}

void Platform::Update(const float deltaTime)
{
    if (horizontalVelocity == 0.f)
        return;

    const float previousX = bounds.position.x;
    bounds.position.x += horizontalVelocity * deltaTime;
    while (bounds.position.x < minimumX || bounds.position.x > maximumX)
    {
        if (bounds.position.x < minimumX)
        {
            bounds.position.x = minimumX + (minimumX - bounds.position.x);
            horizontalVelocity = std::abs(horizontalVelocity);
        }
        else
        {
            bounds.position.x = maximumX - (bounds.position.x - maximumX);
            horizontalVelocity = -std::abs(horizontalVelocity);
        }
    }

    if (door)
        door->MoveHorizontally(bounds.position.x - previousX);
}
