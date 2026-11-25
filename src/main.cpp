
#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    // Création et gestion de la fenêtre
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "ScribbleBounce"
    );

    window.setFramerateLimit(60);

    // Joueur provisoire
    sf::RectangleShape player({40.f, 50.f});
    player.setFillColor(sf::Color::Green);
    player.setPosition({380.f, 500.f});

    while (window.isOpen())
    {
        // Events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Déplacements
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            player.move({-5.f, 0.f});

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            player.move({5.f, 0.f});

        // Affichage
        window.clear(sf::Color(135, 206, 235));

        window.draw(player);

        window.display();
    }

    return 0;
}
