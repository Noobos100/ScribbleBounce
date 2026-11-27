module;
module Platform;

import std;

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
}

Rectangle Platform::GetBounds() const
{
    return bounds;
}

bool Platform::IsHorizontalMovementEligible() const
{
    return horizontalMovementEligible;
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
}
