module;
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

module Player;

namespace
{
    // Accélération verticale vers le bas, en unités du monde par seconde carrée.
    constexpr float gravity = 1800.f;
    // Vitesse verticale initiale vers le haut lors d'un rebond, en unités par seconde.
    constexpr float jumpSpeed = 900.f;
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
    position = {
        windowWidth / 2.f - size.x / 2.f,
        startPlatformTop - size.y
    };
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
    position.x += movement * deltaTime;
    position.y += verticalVelocity * deltaTime;

    if (position.x < 0.f)
        position.x = 0.f;

    if (position.x + size.x > windowWidth)
        position.x = windowWidth - size.x;
}

/**
 * @param window Fenêtre SFML dans laquelle dessiner le joueur.
 */
void Player::Draw(Renderer& renderer) const
{
    renderer.DrawRectangle(GetBounds(), {0, 255, 0});
}

Vector2 Player::GetPosition() const
{
    return position;
}

Rectangle Player::GetBounds() const
{
    return {position, size};
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
    position.y = platformTop - size.y;
    verticalVelocity = -jumpSpeed;
}
