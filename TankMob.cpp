#include "TankMob.h"

TankMob::TankMob(sf::Vector2f startPosition)
    : Mob(startPosition, "tankMob.png")
{
    speed = 60.f;
    hp = 150;
    damage = 20;

    sprite.setScale(0.25f, 0.25f);
}

void TankMob::update(float dt, sf::Vector2f playerPosition)
{
    Mob::update(dt, playerPosition);
}

int TankMob::getPoints() const
{
    return 50;
}