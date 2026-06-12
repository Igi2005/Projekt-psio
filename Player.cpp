#include "Player.h"
#include <iostream>
#include "Weapon.h"
// Konstruktor gracza
// Przyjmuje pozycję startową, na której gracz ma się pojawić na mapie
Player::Player(sf::Vector2f startPosition) : speed(250.f), hp(100), speechTimer(0.f), lookingRight(true)
{
    if (!texture.loadFromFile("player.png")) {
        std::cerr << "Blad: Nie znaleziono player.png" << std::endl;
    }

    // Przypisanie tekstury do sprite'a
    sprite.setTexture(texture);

    // Ustawienie środka sprite'a na jego środek
    // Dzięki temu pozycja gracza oznacza środek obrazka, a nie lewy górny róg
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

    // Ustawienie pozycji startowej gracza
    sprite.setPosition(startPosition);

    // Zmniejszenie rozmiaru sprite'a
    sprite.setScale(0.2f, 0.2f);

    // Wczytanie czcionki do tekstu wypowiadanego przez gracza
    if (!font.loadFromFile("Roboto_Condensed-Black.ttf")) {
        std::cerr << "Blad: Nie znaleziono Roboto_Condensed-Black.ttf" << std::endl;
    }

    // Ustawienie czcionki dla tekstu gracza
    speechText.setFont(font);

    // Ustawienie rozmiaru tekstu
    speechText.setCharacterSize(40);

    // Ustawienie koloru tekstu
    speechText.setFillColor(sf::Color::Yellow);

    // Ustawienie koloru obramowania tekstu
    speechText.setOutlineColor(sf::Color::Black);

    // Ustawienie grubości obramowania tekstu
    speechText.setOutlineThickness(2.f);
}

// Aktualizacja gracza wykonywana co klatkę
void Player::update(float dt, sf::Vector2f mapSize)
{
    // Wektor ruchu gracza
    sf::Vector2f movement(0.f, 0.f);

    // Sterowanie graczem
    // W układzie SFML oś Y rośnie w dół, dlatego ruch do góry odejmuje Y
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x -= speed * dt;
        lookingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        lookingRight = true;
        movement.x += speed * dt;
    }
    // Przesunięcie sprite'a gracza
    sprite.move(movement);

    // Pobranie aktualnej pozycji gracza
    sf::Vector2f pos = sprite.getPosition();

    // Pobranie połowy szerokości i wysokości sprite'a
    // Potrzebne do ograniczenia gracza do granic mapy
    float hw = sprite.getGlobalBounds().width / 2.f;
    float hh = sprite.getGlobalBounds().height / 2.f;

    // Ograniczenie pozycji gracza do lewej krawędzi mapy
    if (pos.x < hw) pos.x = hw;

    // Ograniczenie pozycji gracza do górnej krawędzi mapy
    if (pos.y < hh) pos.y = hh;

    // Ograniczenie pozycji gracza do prawej krawędzi mapy
    if (pos.x > mapSize.x - hw) pos.x = mapSize.x - hw;

    // Ograniczenie pozycji gracza do dolnej krawędzi mapy
    if (pos.y > mapSize.y - hh) pos.y = mapSize.y - hh;

    // Ustawienie poprawionej pozycji gracza
    sprite.setPosition(pos);

    // Obsługa tekstu nad graczem
    // Jeśli speechTimer jest większy od 0, tekst jest widoczny
    if (speechTimer > 0) {
        // Zmniejszanie czasu wyświetlania tekstu
        speechTimer -= dt;

        // Ustawienie tekstu nad graczem
        speechText.setPosition(sprite.getPosition().x - 50, sprite.getPosition().y - 100);
    }
    weapon.update(dt);
}

// Funkcja ustawiająca tekst wypowiadany przez gracza
void Player::say(std::string message) {
    // Ustawienie treści tekstu
    speechText.setString(message);

    // Tekst będzie widoczny przez 2 sekundy
    speechTimer = 2.0f;
}

// Rysowanie gracza
void Player::draw(sf::RenderWindow& window) {
    // Rysowanie sprite'a gracza
    window.draw(sprite);

    // Rysowanie tekstu tylko wtedy, gdy timer jest większy od 0
    if (speechTimer > 0) window.draw(speechText);
}

// Zwraca pozycję gracza
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

// Zwraca aktualne HP gracza
int Player::getHp() const {
    return hp;
}

// Zadawanie obrażeń graczowi
void Player::takeDamage(int damage) {
    // Odjęcie HP
    hp -= damage;

    // Zabezpieczenie przed zejściem HP poniżej 0
    if (hp < 0) hp = 0;
}

// Zwraca prostokąt kolizji gracza
// Używane np. do sprawdzania kolizji z mobami
sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

// Sprawdza, czy gracz nadal żyje
bool Player::isAlive() const {
    return hp > 0;
}

Bullet Player::shoot()
{
    if (lookingRight)
    {
        return weapon.shoot(getPosition(), sf::Vector2f(1.f, 0.f));
    }
    else
    {
        return weapon.shoot(getPosition(), sf::Vector2f(-1.f, 0.f));
    }
}

bool Player::canShoot() const
{
    return weapon.canShoot();
}

void Player::reset(sf::Vector2f startPosition)
{
    sprite.setPosition(startPosition);

    hp = 100;
    speechTimer = 0.f;
    lookingRight = true;

    weapon.reset();
}