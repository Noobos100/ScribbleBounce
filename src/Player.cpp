module;
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

module Player;

namespace
{
    constexpr float gravity = 1800.f;
    constexpr float jumpSpeed = 700.f;
}

Player::Player(const float windowWidth, const float groundLevel)
    : speed(300.f),
      windowWidth(windowWidth),
      groundLevel(groundLevel),
      verticalVelocity(-jumpSpeed)
{
    shape.setSize({40.f, 50.f});
    shape.setFillColor(sf::Color::Green);

    shape.setPosition({
        windowWidth / 2.f - 20.f,
        groundLevel - shape.getSize().y
    });
}

void Player::update(const float deltaTime)
{
    float movement = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        movement -= speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        movement += speed;

    verticalVelocity += gravity * deltaTime;
    shape.move({movement * deltaTime, verticalVelocity * deltaTime});

    auto position = shape.getPosition();

    if (position.x < 0.f)
        position.x = 0.f;

    if (position.x + shape.getSize().x > windowWidth)
        position.x = windowWidth - shape.getSize().x;

    if (position.y + shape.getSize().y >= groundLevel && verticalVelocity > 0.f)
    {
        position.y = groundLevel - shape.getSize().y;
        verticalVelocity = -jumpSpeed;
    }

    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::Vector2f Player::GetPosition() const
{
    return shape.getPosition();
}
