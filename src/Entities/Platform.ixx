module;
export module Platform;

import Renderer;

export class Platform
{
    Rectangle bounds;

public:
    Platform(float x, float y, float width, float height);

    void Draw(Renderer& renderer) const;
    Rectangle GetBounds() const;
};
