module;
#include <SFML/Graphics.hpp>

export module Game;

import std;
import Platform;
import Player;

export class Game
{
    std::vector<Platform> platforms;
    Player player;

public:
    explicit Game(float windowWidth);

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window) const;
};
