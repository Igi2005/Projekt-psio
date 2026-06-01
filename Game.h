#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.h"
#include "Map.h"
#include "Mob.h"
#include "FastMob.h"
#include "TankMob.h"

class Game
{
private:
    sf::RenderWindow window;
    sf::View worldView;

    Player player;
    Map map;

    std::vector<Mob*> mobs;

    sf::Clock clock;

    float spawnTimer;
    float spawnCooldown;

    void processEvents();
    void update(float dt);
    void render();
    void updateCamera();
    void spawnMob();

public:
    Game();
    ~Game();

    void run();
};

#endif