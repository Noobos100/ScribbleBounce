
#include <SFML/Graphics.hpp>
import std;
import Game;

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
    Game game(static_cast<float>(width));

    sf::Clock clock;

    while (window.isOpen())
    {
        // Events s'il y en a
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        const float deltaTime = clock.restart().asSeconds();
        game.Update(deltaTime);

        // Affichage
        window.clear(sf::Color(135, 206, 235));
        game.Draw(window);
        window.display();
    }

    return 0;
}
