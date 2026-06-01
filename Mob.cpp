#include "Mob.h"

#include <iostream>
#include <cmath>

Mob::Mob(sf::Vector2f startPosition, const std::string& texturePath)
    : speed(100.f), hp(50), damage(10), alive(true)
{
    if (!texture.loadFromFile(texturePath))
    {
        std::cerr << "Failed to load " << texturePath << std::endl;
    }

    sprite.setTexture(texture);

    sprite.setOrigin(
        texture.getSize().x / 2.f,
        texture.getSize().y / 2.f
    );

    sprite.setPosition(startPosition);
    sprite.setScale(0.15f, 0.15f);
}

Mob::~Mob()
{
}

void Mob::update(float dt, sf::Vector2f playerPosition)
{
    sf::Vector2f position = sprite.getPosition();

    sf::Vector2f direction(
        playerPosition.x - position.x,
        playerPosition.y - position.y
    );

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0.f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    sprite.move(direction * speed * dt);
}

void Mob::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Mob::takeDamage(int value)
{
    hp -= value;

    if (hp <= 0)
    {
        alive = false;
    }
}

bool Mob::isAlive() const
{
    return alive;
}

sf::Vector2f Mob::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect Mob::getBounds() const
{
    return sprite.getGlobalBounds();
}

int Mob::getPoints() const
{
    return 10;
}