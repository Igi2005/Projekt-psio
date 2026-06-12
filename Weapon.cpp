#include "Weapon.h"

Weapon::Weapon()
    : cooldown(0.35f),
      cooldownTimer(0.35f)
{
}

// Aktualizuje licznik cooldownu broni
void Weapon::update(float dt)
{
    cooldownTimer += dt;
}

// Sprawdza, czy broń może już oddać kolejny strzał
bool Weapon::canShoot() const
{
    return cooldownTimer >= cooldown;
}

// Tworzy pocisk w podanym kierunku i resetuje cooldown
Bullet Weapon::shoot(sf::Vector2f startPosition, sf::Vector2f direction)
{
    cooldownTimer = 0.f;

    return Bullet(startPosition, direction);
}

void Weapon::reset()
{
    cooldownTimer = cooldown;
}