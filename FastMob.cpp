#include "FastMob.h"

FastMob::FastMob(sf::Vector2f startPosition): Mob(startPosition, "fast_mob.png")
{
    speed = 180.f;
    hp = 25;
    damage = 5;
    sprite.setScale(0.1f, 0.1f);
}

void FastMob::update(float dt, sf::Vector2f playerPosition)
{
    Mob::update(dt, playerPosition);
}

int FastMob::getPoints() const
{
    return 20;
}