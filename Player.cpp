#include "Player.h"
#include <iostream>

//konstruktor do wczytania awataru gracza z lista inicjalizacyjna
Player::Player(sf::Vector2f startPosition): speed(250.f), hp(100)
{
    if (!texture.loadFromFile("player.png"))
    {
        std::cerr << "Failed to load player.png" << std::endl;
    }

    sprite.setTexture(texture);

    // normalnie pozycja sprite jest liczona od lewego górnego rogu
    // po tej linijce pozycja gracza będzie liczona od środka postaci

    sprite.setOrigin(
        texture.getSize().x / 2.f,
        texture.getSize().y / 2.f
    );

    sprite.setPosition(startPosition);

    //zmniejszenie do 20% rozmiaru
    sprite.setScale(0.2f, 0.2f);
}

void Player::update(float dt, sf::Vector2f mapSize)
{
    sf::Vector2f movement(0.f, 0.f);

    //ruszanie sie po mapie
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed * dt;

    sprite.move(movement);

    sf::Vector2f position = sprite.getPosition();

    float halfWidth =
        sprite.getGlobalBounds().width / 2.f;

    float halfHeight =
        sprite.getGlobalBounds().height / 2.f;

    //zeby za mape nie wyszedl ograniczenia
    if (position.x < halfWidth)
        position.x = halfWidth;

    if (position.y < halfHeight)
        position.y = halfHeight;

    if (position.x > mapSize.x - halfWidth)
        position.x = mapSize.x - halfWidth;

    if (position.y > mapSize.y - halfHeight)
        position.y = mapSize.y - halfHeight;

    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

int Player::getHp() const
{
    return hp;
}