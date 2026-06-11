#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

#include <string>
#include "Player.h"
#include "Map.h"
#include "Mob.h"
#include "FastMob.h"
#include "TankMob.h"
#include <SFML/Audio.hpp>
#include "Bullet.h"

class Game
{
private:
    //Okno gry, do którego rysujemy mapę, gracza, moby i HUD.
    sf::RenderWindow window;
    //Kamera gry.
    sf::View worldView;
    Player player;
    //Obiekt mapy, czyli obrazek mapa.png.
    Map map;
    std::vector<Mob*> mobs;
    sf::Clock clock;
    //liczy czas od ostatniego spawnu,
    float spawnTimer;
    //mówi, co ile sekund ma pojawić się nowy mob.
    float spawnCooldown;
    sf::Font font;
    sf::Text hudText;
    std::string playerName;
    int score;
    //do spawnu mobow i licznika czasu
    float gameTime;
    sf::SoundBuffer eSoundBuffer;
    sf::Sound eSound;
    void processEvents();
    void update(float dt);
    void render();
    void updateCamera();
    void spawnMob();
    void updateHud();
    void drawHud();
    std::vector<Bullet> bullets;
    void updateBullets(float dt);
    void checkBulletMobCollisions();
    void removeDeadMobs();

public:
    Game();
    ~Game();

    void run();
};

#endif