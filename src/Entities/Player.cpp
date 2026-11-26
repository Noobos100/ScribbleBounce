module;
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

module Player;

namespace
{
    // Accélération verticale vers le bas, en unités du monde par seconde carrée.
    constexpr float gravity = 1800.f;
    // Vitesse verticale initiale vers le haut lors d'un rebond, en unités par seconde.
    constexpr float jumpSpeed = 800.f;
}

/**
 * @param windowWidth Largeur de la fenêtre en unités du monde, utilisée pour limiter
 *                    le déplacement horizontal.
 * @param startPlatformTop Coordonnée verticale du haut de la plateforme de départ.
 */
Player::Player(const float windowWidth, const float startPlatformTop)
    : speed(300.f),
      windowWidth(windowWidth),
      verticalVelocity(0.f)
{
    shape.setSize({40.f, 50.f});
    shape.setFillColor(sf::Color::Green);

    shape.setPosition({
        windowWidth / 2.f - 20.f,
        startPlatformTop - shape.getSize().y
    });
}

/**
 * @param deltaTime Temps écoulé depuis la mise à jour précédente, en secondes.
 */
void Player::update(const float deltaTime)
{
    float movement = 0.f;

    // déplacements au clavier
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

    shape.setPosition(position);
}

/**
 * @param window Fenêtre SFML dans laquelle dessiner le joueur.
 */
void Player::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::Vector2f Player::GetPosition() const
{
    return shape.getPosition();
}

sf::FloatRect Player::GetBounds() const
{
    return shape.getGlobalBounds();
}

bool Player::IsFalling() const
{
    return verticalVelocity > 0.f;
}

/**
 * @param platformTop Coordonnée verticale du haut de la plateforme sur laquelle le joueur
 *                    vient d'atterrir.
 */
void Player::LandOn(const float platformTop)
{
    auto position = shape.getPosition();
    position.y = platformTop - shape.getSize().y;
    shape.setPosition(position);
    verticalVelocity = -jumpSpeed;
}
