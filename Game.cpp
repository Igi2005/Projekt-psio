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
      score(0)
{
    srand(static_cast<unsigned>(time(nullptr)));
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    hudText.setFont(font);
    hudText.setCharacterSize(28);
    hudText.setFillColor(sf::Color::White);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    window.create(
        desktop,
        "Survival Arena",
        sf::Style::Fullscreen
    );

    window.setFramerateLimit(60);

    worldView = window.getDefaultView();
    worldView.setCenter(player.getPosition());
}

Game::~Game()
{
    for (Mob* mob : mobs)
    {
        delete mob;
    }

    mobs.clear();
}

void Game::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
    }
}

void Game::update(float dt)
{
    player.update(dt, map.getSize());

    updateCamera();

    spawnTimer += dt;

    if (spawnTimer >= spawnCooldown)
    {
        spawnMob();
        spawnTimer = 0.f;
    }

    for (Mob* mob : mobs)
    {
        mob->update(dt, player.getPosition());
    }
}

void Game::render()
{
    window.clear();

    window.setView(worldView);

    map.draw(window);
    player.draw(window);

    for (Mob* mob : mobs)
    {
        mob->draw(window);
    }

    drawHud();

    window.display();
}

void Game::updateCamera()
{
    sf::Vector2f playerPosition = player.getPosition();
    sf::Vector2f mapSize = map.getSize();
    sf::Vector2f viewSize = worldView.getSize();

    float halfWidth = viewSize.x / 2.f;
    float halfHeight = viewSize.y / 2.f;

    float cameraX = playerPosition.x;
    float cameraY = playerPosition.y;

    if (cameraX < halfWidth)
    {
        cameraX = halfWidth;
    }

    if (cameraY < halfHeight)
    {
        cameraY = halfHeight;
    }

    if (cameraX > mapSize.x - halfWidth)
    {
        cameraX = mapSize.x - halfWidth;
    }

    if (cameraY > mapSize.y - halfHeight)
    {
        cameraY = mapSize.y - halfHeight;
    }

    worldView.setCenter(cameraX, cameraY);
}

void Game::spawnMob()
{
    sf::Vector2f mapSize = map.getSize();

    sf::Vector2f viewCenter = worldView.getCenter();
    sf::Vector2f viewSize = worldView.getSize();

    float left = viewCenter.x - viewSize.x / 2.f;
    float right = viewCenter.x + viewSize.x / 2.f;
    float top = viewCenter.y - viewSize.y / 2.f;
    float bottom = viewCenter.y + viewSize.y / 2.f;

    sf::Vector2f position;

    do
    {
        position.x = static_cast<float>(rand() % static_cast<int>(mapSize.x));
        position.y = static_cast<float>(rand() % static_cast<int>(mapSize.y));

    } while (
        position.x > left &&
        position.x < right &&
        position.y > top &&
        position.y < bottom
    );

    int randomType = rand() % 3;

    if (randomType == 0)
    {
        mobs.push_back(new Mob(position, "mob.png"));
    }
    else if (randomType == 1)
    {
        mobs.push_back(new FastMob(position));
    }
    else
    {
        mobs.push_back(new TankMob(position));
    }
}

void Game::updateHud()
{
    std::stringstream ss;

    ss << "Player: " << playerName << "\n";
    ss << "HP: " << player.getHp() << "\n";
    ss << "Score: " << score;

    hudText.setString(ss.str());

    sf::FloatRect bounds = hudText.getLocalBounds();

    float x = window.getSize().x - bounds.width - 30.f;
    float y = 30.f;

    hudText.setPosition(x, y);
}

void Game::drawHud()
{
    window.setView(window.getDefaultView());

    updateHud();

    window.draw(hudText);
}