#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Map.h"

class Game
{
private:
    sf::RenderWindow window;

    sf::View worldView;

    Player player;

    Map map;

    sf::Clock clock;

    void processEvents();

    void update(float dt);

    void render();

    void updateCamera();

public:
    Game();

    void run();
};

#endif