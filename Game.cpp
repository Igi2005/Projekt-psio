#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

Game::Game()
    : player(sf::Vector2f(
        sf::VideoMode::getDesktopMode().width / 2.f,
        sf::VideoMode::getDesktopMode().height / 2.f
    )),
      spawnTimer(0.f),
      spawnCooldown(2.f),
      playerName("Robert"),
      score(0),
      gameTime(0.0f)
{
    srand(static_cast<unsigned>(time(nullptr)));
    if (!font.loadFromFile("Roboto_Condensed-Black.ttf")) {
        std::cerr << "Blad: Nie znaleziono Roboto_Condensed-Black.ttf" << std::endl;
    }
    hudText.setFont(font);
    hudText.setCharacterSize(28);
    hudText.setFillColor(sf::Color::White);
    window.create(sf::VideoMode(1280, 720), "Survival Arena");
    window.setFramerateLimit(60);
    worldView = window.getDefaultView();
}

Game::~Game() { for (Mob* mob : mobs) delete mob; mobs.clear(); }

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
    }
}


void Game::update(float dt)
{    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
    player.say("Zmykaj!");
}

    gameTime += dt;
    if (gameTime < 60.f)
    {
        spawnCooldown = 2.f;
    }
    else if (gameTime < 120.f)
    {
        spawnCooldown = 1.5f;
    }
    else if (gameTime < 180.f)
    {
        spawnCooldown = 1.f;
    }
    else
    {
        spawnCooldown = 0.5f;
    }
    //aktualizuje gracza
    player.update(dt, map.getSize());
    //update kamery zeby za graczem szla

    updateCamera();
    spawnTimer += dt;
    if (spawnTimer >= spawnCooldown) { spawnMob(); spawnTimer = 0.f; }
    for (Mob* mob : mobs) { mob->update(dt, player.getPosition()); mob->attack(player, dt); }
    if (!player.isAlive()) window.close();
}

void Game::render() {
    window.clear();
    window.setView(worldView);
    map.draw(window);
    player.draw(window);
    for (Mob* mob : mobs) { mob->draw(window); }
    drawHud();
    window.display();
}

void Game::updateCamera() {
    sf::Vector2f pos = player.getPosition();
    sf::Vector2f mapSize = map.getSize();
    sf::Vector2f viewSize = worldView.getSize();
    float cx = std::max(viewSize.x / 2.f, std::min(pos.x, mapSize.x - viewSize.x / 2.f));
    float cy = std::max(viewSize.y / 2.f, std::min(pos.y, mapSize.y - viewSize.y / 2.f));
    worldView.setCenter(cx, cy);
}

void Game::spawnMob() {
    sf::Vector2f mapSize = map.getSize();
    sf::Vector2f pos(static_cast<float>(rand() % (int)mapSize.x), static_cast<float>(rand() % (int)mapSize.y));
    mobs.push_back(new Mob(pos, "mob.png"));
}

void Game::updateHud() {
    std::stringstream ss;

    int minutes = static_cast<int>(gameTime) / 60;
    int seconds = static_cast<int>(gameTime) % 60;
    ss << "Player: " << playerName << "\n";
    ss << "HP: " << player.getHp() << "\n";
    ss << "Score: " << score << "\n";
    ss << "Time: " << minutes << ":";
    if (seconds < 10)
    {
        ss << "0";
    }

    ss << seconds << "\n";
    hudText.setString(ss.str());
    hudText.setPosition(window.getView().getCenter().x + 400, window.getView().getCenter().y - 300);
}

void Game::drawHud() {
    updateHud();
    window.draw(hudText);
}