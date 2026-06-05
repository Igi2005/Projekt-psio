#include "Mob.h"
#include "Player.h"
#include <iostream>
#include <cmath>

Mob::Mob(sf::Vector2f startPosition, const std::string& texturePath)
: speed(100.f), hp(50), damage(10), alive(true), attackTimer(0.f), attackCooldown(1.f)
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
    //gracz: (500, 300)
    //mob:   (100, 200)
    //direction = (500 - 100, 300 - 200)
    //direction = (400, 100)
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
    //ruch do gracza
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
//Zwraca pozycję moba.
sf::Vector2f Mob::getPosition() const
{
    return sprite.getPosition();
}

//Zwraca prostokąt kolizji sprite’a.
sf::FloatRect Mob::getBounds() const
{
    return sprite.getGlobalBounds();
}

int Mob::getPoints() const
{
    return 10;
}

void Mob::attack(Player& player, float dt)
{
    attackTimer += dt;
    //getBounds zwraca kordy do kolizji, intersects() sprawdza, czy te dwa prostokąty się przecinają
    if (getBounds().intersects(player.getBounds()) && attackTimer >= attackCooldown)
    {
        player.takeDamage(damage);
        //reset cooldawnu do ataku moba
        attackTimer = 0.f;
    }
}