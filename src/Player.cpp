module;
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

module Player;

Player::Player(const float windowWidth, const float windowHeight)
    : speed(300.f),
      windowWidth(windowWidth),
      windowHeight(windowHeight)
{
    shape.setSize({40.f, 50.f});
    shape.setFillColor(sf::Color::Green);

    shape.setPosition({
        windowWidth / 2.f - 20.f,
        windowHeight - 100.f
    });
}

void Player::update(const float deltaTime)
{
    float movement = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        movement -= speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        movement += speed;

    shape.move({movement * deltaTime, 0.f});

    auto position = shape.getPosition();

    if (position.x < 0.f)
        position.x = 0.f;

    if (position.x + shape.getSize().x > windowWidth)
        position.x = windowWidth - shape.getSize().x;

    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}
