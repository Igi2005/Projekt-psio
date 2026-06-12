#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <algorithm>

Game::Game()
    : player(sf::Vector2f(
        sf::VideoMode::getDesktopMode().width / 2.f,
        sf::VideoMode::getDesktopMode().height / 2.f
    )),
      spawnTimer(0.f),
      spawnCooldown(2.f),
      gameTime(0.f),
      playerName("Robert"),
      score(0),
      gameOver(false),
      resultSaved(false),
      recordManager("data.csv")
{
    srand(static_cast<unsigned>(time(nullptr)));

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    window.create(
        desktop,
        "Survival Arena",
        sf::Style::Fullscreen
    );

    window.setFramerateLimit(60);

    worldView = window.getDefaultView();

    if (!font.loadFromFile("Roboto_Condensed-Black.ttf"))
    {
        std::cerr << "Blad: Nie znaleziono Roboto_Condensed-Black.ttf" << std::endl;
    }

    hudText.setFont(font);
    hudText.setCharacterSize(28);
    hudText.setFillColor(sf::Color::White);
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(26);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(80.f, 80.f);

    if (!eSoundBuffer.loadFromFile("zmykaj.wav"))
    {
        std::cerr << "Blad: Nie znaleziono zmykaj.wav" << std::endl;
    }

    eSound.setBuffer(eSoundBuffer);
    eSound.setVolume(80.f);
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

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }

            if (event.key.code == sf::Keyboard::E)
            {
                player.say("Zmykaj!");
                eSound.play();
            }
            if (event.key.code == sf::Keyboard::R && gameOver)
            {
                resetGame();
            }
        }
    }
}

void Game::update(float dt)
{
    if (gameOver)
    {
        return;
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

    player.update(dt, map.getSize());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player.canShoot())
    {
        bullets.push_back(player.shoot());
    }

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
        mob->attack(player, dt);
    }

    updateBullets(dt);
    checkBulletMobCollisions();
    removeDeadMobs();

    if (!player.isAlive())
    {
        if (!resultSaved)
        {
            recordManager.saveResult(playerName, gameTime, score);
            recordManager.setData();
            records = recordManager.getData();

            resultSaved = true;
            gameOver = true;
        }
    }
}

void Game::render()
{
    window.clear();
    if (gameOver)
    {
        renderGameOver();
        window.display();
        return;
    }
    window.setView(worldView);

    map.draw(window);
    player.draw(window);

    for (Mob* mob : mobs)
    {
        mob->draw(window);
    }

    for (Bullet& bullet : bullets)
    {
        bullet.draw(window);
    }

    drawHud();

    window.display();
}

void Game::updateCamera()
{
    sf::Vector2f playerPosition = player.getPosition();
    sf::Vector2f mapSize = map.getSize();
    sf::Vector2f viewSize = worldView.getSize();

    float cameraX = std::max(
        viewSize.x / 2.f,
        std::min(playerPosition.x, mapSize.x - viewSize.x / 2.f)
    );

    float cameraY = std::max(
        viewSize.y / 2.f,
        std::min(playerPosition.y, mapSize.y - viewSize.y / 2.f)
    );

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

    sf::Vector2f pos;

    do
    {
        pos.x = static_cast<float>(rand() % static_cast<int>(mapSize.x));
        pos.y = static_cast<float>(rand() % static_cast<int>(mapSize.y));

    } while (
        pos.x > left &&
        pos.x < right &&
        pos.y > top &&
        pos.y < bottom
    );

    int randomType = rand() % 3;

    if (randomType == 0)
    {
        mobs.push_back(new Mob(pos, "mob.png"));
    }
    else if (randomType == 1)
    {
        mobs.push_back(new FastMob(pos));
    }
    else
    {
        mobs.push_back(new TankMob(pos));
    }
}

void Game::updateBullets(float dt)
{
    for (Bullet& bullet : bullets)
    {
        bullet.update(dt);
    }

    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](const Bullet& bullet)
            {
                return !bullet.isActive();
            }
        ),
        bullets.end()
    );
}

void Game::checkBulletMobCollisions()
{
    for (Bullet& bullet : bullets)
    {
        if (!bullet.isActive())
        {
            continue;
        }

        for (Mob* mob : mobs)
        {
            if (mob->isAlive() && bullet.getBounds().intersects(mob->getBounds()))
            {
                mob->takeDamage(bullet.getDamage());
                bullet.deactivate();
                break;
            }
        }
    }
}

void Game::removeDeadMobs()
{
    for (auto it = mobs.begin(); it != mobs.end(); )
    {
        if (!(*it)->isAlive())
        {
            score += (*it)->getPoints();

            delete *it;

            it = mobs.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::updateHud()
{
    std::stringstream ss;

    int minutes = static_cast<int>(gameTime) / 60;
    int seconds = static_cast<int>(gameTime) % 60;

    ss << "Player: " << playerName << "\n";

    ss << "Time: " << minutes << ":";

    if (seconds < 10)
    {
        ss << "0";
    }

    ss << seconds << "\n";

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

void Game::renderGameOver()
{
    window.setView(window.getDefaultView());

    std::stringstream ss;

    ss << "KONIEC GRY\n\n";
    ss << "Twoj wynik:\n";
    ss << "Player: " << playerName << "\n";

    int minutes = static_cast<int>(gameTime) / 60;
    int seconds = static_cast<int>(gameTime) % 60;

    ss << "Time: " << minutes << ":";

    if (seconds < 10)
    {
        ss << "0";
    }

    ss << seconds << "\n";
    ss << "Score: " << score << "\n\n";

    ss << "Lista wynikow z pliku:\n\n";

    for (std::string line : records)
    {
        ss << line << "\n";
    }

    ss << "\nNacisnij R, aby zagrac ponownie.";
    ss << "\nNacisnij ESC, aby zamknac gre.";

    gameOverText.setString(ss.str());

    window.draw(gameOverText);
}

void Game::resetGame()
{
    for (Mob* mob : mobs)
    {
        delete mob;
    }

    mobs.clear();
    bullets.clear();

    gameTime = 0.f;
    spawnTimer = 0.f;
    spawnCooldown = 2.f;
    score = 0;

    gameOver = false;
    resultSaved = false;

    records.clear();

    player.reset(sf::Vector2f(
        sf::VideoMode::getDesktopMode().width / 2.f,
        sf::VideoMode::getDesktopMode().height / 2.f
    ));

    worldView = window.getDefaultView();
}