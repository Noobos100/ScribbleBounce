
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
import std;
import Game;
import SfmlRenderer;

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
    SfmlRenderer renderer(window);
    Game game(static_cast<float>(width), static_cast<float>(height));

    sf::Clock clock;

    while (window.isOpen())
    {
        // Events s'il y en a
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::R)
                    game.Restart();
                else if (keyPressed->code == sf::Keyboard::Key::C)
                    game.Continue();
            }
        }

        const float deltaTime = clock.restart().asSeconds();
        game.Update(deltaTime);

        // Affichage
        renderer.Clear({135, 206, 235});
        game.Draw(renderer);
        renderer.Display();
    }

    return 0;
}
