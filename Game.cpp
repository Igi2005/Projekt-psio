#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <algorithm>

Game::Game()
    : player(sf::Vector2f(
        sf::VideoMode(1280, 720).width / 2.f,
        sf::VideoMode(1280, 720).height / 2.f
    )),
      spawnTimer(0.f),
      spawnCooldown(2.f),
      gameTime(0.f),
      playerName("Robert"),
      score(0)
{
    // Ustawienie losowania, żeby moby nie pojawiały się zawsze w tych samych miejscach
    srand(static_cast<unsigned>(time(nullptr)));

    // Wczytanie czcionki używanej do HUD-a
    if (!font.loadFromFile("Roboto_Condensed-Black.ttf"))
    {
        std::cerr << "Blad: Nie znaleziono Roboto_Condensed-Black.ttf" << std::endl;
    }
    if (!eSoundBuffer.loadFromFile("zmykaj.wav"))
    {
        std::cerr << "Blad: Nie znaleziono zmykaj.wav" << std::endl;
    }

    eSound.setBuffer(eSoundBuffer);
    eSound.setVolume(80.f);

    // Ustawienie wyglądu tekstu HUD
    hudText.setFont(font);
    hudText.setCharacterSize(28);
    hudText.setFillColor(sf::Color::White);

    // Tworzenie okna gry
    window.create(sf::VideoMode(1280, 720), "Survival Arena");
    window.setFramerateLimit(60);

    // Pobranie domyślnej kamery okna
    worldView = window.getDefaultView();

    // Ustawienie kamery na pozycję gracza
    worldView.setCenter(player.getPosition());
}

Game::~Game()
{
    // Usuwanie mobów utworzonych przez new
    for (Mob* mob : mobs)
    {
        delete mob;
    }

    mobs.clear();
}

void Game::run()
{
    // Główna pętla gry
    while (window.isOpen())
    {
        // dt = czas między klatkami
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            if (event.key.code == sf::Keyboard::E) {
                player.say("Zmykaj!");
                eSound.play();
            }
        }
    }
}

void Game::update(float dt)
{

    // Zwiększanie całkowitego czasu gry
    gameTime += dt;

    // Im dłużej gracz żyje, tym częściej pojawiają się przeciwnicy
    if (gameTime < 60.f)
    {
        spawnCooldown = 2.f;
    }
    else if (gameTime < 120.f)
    {
        spawnCooldown = 1.5f;
    }aaaaaaaaaa
    else if (gameTime < 180.f)
    {
        spawnCooldown = 1.f;
    }
    else
    {
        spawnCooldown = 0.5f;
    }

    // Aktualizacja gracza i ograniczenie jego pozycji do mapy
    player.update(dt, map.getSize());

    // Aktualizacja kamery, żeby podążała za graczem
    updateCamera();

    // Licznik czasu od ostatniego spawnu
    spawnTimer += dt;

    // Jeżeli minął cooldown, tworzymy nowego moba
    if (spawnTimer >= spawnCooldown)
    {
        spawnMob();
        spawnTimer = 0.f;
    }

    // Aktualizacja mobów:
    // każdy mob idzie w stronę gracza i sprawdza, czy może go zaatakować
    for (Mob* mob : mobs)
    {
        mob->update(dt, player.getPosition());
        mob->attack(player, dt);
    }

    // Jeżeli gracz nie żyje, zamykamy grę
    if (!player.isAlive())
    {
        window.close();
    }
}

void Game::render()
{
    window.clear();

    // Ustawienie kamery świata gry
    window.setView(worldView);

    // Rysowanie obiektów świata gry
    map.draw(window);
    player.draw(window);

    for (Mob* mob : mobs)
    {
        mob->draw(window);
    }

    // Rysowanie HUD-a na domyślnym widoku ekranu
    drawHud();

    window.display();
}

void Game::updateCamera()
{
    sf::Vector2f pos = player.getPosition();
    sf::Vector2f mapSize = map.getSize();
    sf::Vector2f viewSize = worldView.getSize();

    // Ograniczenie środka kamery do granic mapy
    float cx = std::max(
        viewSize.x / 2.f,
        std::min(pos.x, mapSize.x - viewSize.x / 2.f)
    );

    float cy = std::max(
        viewSize.y / 2.f,
        std::min(pos.y, mapSize.y - viewSize.y / 2.f)
    );

    worldView.setCenter(cx, cy);
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

    // Losowanie pozycji poza aktualnym widokiem kamery
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

    // Losowanie typu przeciwnika
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

    // Pobranie rozmiaru tekstu, żeby ustawić go przy prawej krawędzi
    sf::FloatRect bounds = hudText.getLocalBounds();

    // Ustawienie tekstu w prawym górnym rogu
    float x = window.getSize().x - bounds.width - 30.f;
    float y = 30.f;

    hudText.setPosition(x, y);
}

void Game::drawHud()
{
    // Przełączenie na domyślny widok okna.
    // Dzięki temu HUD jest przyklejony do ekranu, a nie do mapy.
    window.setView(window.getDefaultView());

    updateHud();

    window.draw(hudText);
}