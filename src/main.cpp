
#include <SFML/Graphics.hpp>
import std;
import Player;

int main()
{
    // Création et gestion de la fenêtre
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;

    sf::RenderWindow window(
        sf::VideoMode({width, height}),
        "ScribbleBounce"
    );

    window.setFramerateLimit(60);

    // Joueur
    Player player(width, height);
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
        player.draw(window);
        window.display();
    }

    return 0;
}
