
#include <SFML/Graphics.hpp>
import std;
import Platform;
import Player;

int main()
{
    // Création et gestion de la fenêtre
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;
    constexpr float groundHeight = 20.f;

    sf::RenderWindow window(
        sf::VideoMode({width, height}),
        "ScribbleBounce"
    );

    window.setFramerateLimit(60);

    const float groundLevel = static_cast<float>(height) - groundHeight;
    Player player(static_cast<float>(width), groundLevel);

    sf::RectangleShape ground({
        static_cast<float>(width),
        groundHeight
    });
    ground.setFillColor(sf::Color(90, 180, 75));
    ground.setPosition({0.f, groundLevel});

    sf::Clock clock;

    while (window.isOpen())
    {
        // Events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        const float deltaTime = clock.restart().asSeconds();

        player.update(deltaTime);

        // Affichage
        window.clear(sf::Color(135, 206, 235));
        window.draw(ground);
        player.draw(window);
        window.display();
    }

    return 0;
}
