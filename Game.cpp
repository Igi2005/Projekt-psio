#include "Game.h"


//konstruktor  gry
// :player() oznacza ze player() jest tworzony od razu przy tworzeniu obiektu GAME
//argumenty mówia ze pobieramy wielkosc ekranu usera i stawiamy ludka mniej wiecej w polowie
Game::Game() : player(sf::Vector2f(sf::VideoMode::getDesktopMode().width / 2.f,sf::VideoMode::getDesktopMode().height / 2.f))
{
    //zmienna ktora przechowuje info o ekranie
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    //tworzenie okienka gry arg1 - okno ma rozmiar jak ekran usera arg2 nazwa arg3 tryb pelnyekran
    window.create(desktop,"Survival Arena",sf::Style::Fullscreen);
    //limit klatek zeby kompa nie spalic
    window.setFramerateLimit(60);
    //pobranie widoku z kamery
    worldView = window.getDefaultView();
    //ustawienie na pozycje gracza
    worldView.setCenter(player.getPosition());
}

//uruchamia petle gry
void Game::run()
{
    //dopki okno jest otwarte
    while (window.isOpen())
    {
        //liczenie czasu jaki minal od poprzedniej klatki
        float dt = clock.restart().asSeconds();

        processEvents();

        update(dt);

        render();
    }
}

//Funkcja do zamykania okna
void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        //czy user kliknal x w prawym gornym
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed & event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
    }
}

void Game::update(float dt)
{
    player.update(dt, map.getSize());

    updateCamera();
}

//funkcja zeby kamera sie poruszala za userem
void Game::updateCamera()
{
    //pobranie pozycji
    sf::Vector2f playerPosition = player.getPosition();

    //pobranie wielkosci mapy
    sf::Vector2f mapSize = map.getSize();
    //pobranie widoku kamery
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

//rysowanie mapy gracza oraz kamery
void Game::render()
{
    window.clear();

    window.setView(worldView);

    map.draw(window);

    player.draw(window);

    window.display();
}