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

    //sprawdza wszystkie eventy z okna
    while (window.pollEvent(event))
    {
        //klikniecie x
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        //klikniemy escape
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
    }
}

void Game::update(float dt)
{
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
    //dodaje czas klatki do licznika spawnu.
    spawnTimer += dt;
    //Aktualizuje wszystkie moby.
    //Każdy mob dostaje:
    //dt,
    //pozycję gracza.
    //Dzięki temu mob wie, w którą stronę ma iść.
    if (spawnTimer >= spawnCooldown)
    {
        spawnMob();
        spawnTimer = 0.f;
    }

    for (Mob* mob : mobs)
    {
        mob->update(dt, player.getPosition());
        mob->attack(player, dt);
    }

    if (!player.isAlive())
    {
        window.close();
    }
}

void Game::render()
{
    //Czyści ekran przed narysowaniem nowej klatki.
    window.clear();

    //Ustawia kamerę świata
    window.setView(worldView);

    //Rysuje mapę i gracza
    map.draw(window);
    player.draw(window);

    //rysowanie mobow
    for (Mob* mob : mobs)
    {
        mob->draw(window);
    }

    drawHud();

    window.display();
}

void Game::updateCamera()
{
    //pobranie pozycji gracza
    sf::Vector2f playerPosition = player.getPosition();
    //pobranie rozmiaru mapy
    sf::Vector2f mapSize = map.getSize();
    //pobranie rozmiaru kamery
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

    //pobranie sroda kamery
    sf::Vector2f viewCenter = worldView.getCenter();
    //pobranie rozmiaru kamery
    sf::Vector2f viewSize = worldView.getSize();

    //pobranie granic kamery
    float left = viewCenter.x - viewSize.x / 2.f;
    float right = viewCenter.x + viewSize.x / 2.f;
    float top = viewCenter.y - viewSize.y / 2.f;
    float bottom = viewCenter.y + viewSize.y / 2.f;
    //Tworzy zmienną na pozycję nowego moba.
    sf::Vector2f position;
    //Losuje pozycję moba na mapie.
    //Pętla działa tak długo, aż pozycja będzie poza aktualnym widokiem kamery.
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
    //strumien ktory wyswietlamy na mapie
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
    //Pobiera rozmiar tekstu.
    sf::FloatRect bounds = hudText.getLocalBounds();
    //ustawienie w prawym gornym rogu
    float x = window.getSize().x - bounds.width - 30.f;
    float y = 30.f;

    hudText.setPosition(x, y);
}

void Game::drawHud()
{
    //Przełącza kamerę na domyślny widok ekranu.
    window.setView(window.getDefaultView());

    updateHud();

    window.draw(hudText);
}