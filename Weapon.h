#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Weapon
{
private:
    float cooldown;       // minimalny czas między strzałami
    float cooldownTimer;  // licznik czasu od ostatniego strzału

public:
    Weapon();

    void update(float dt);
    bool canShoot() const;
    void reset();

    Bullet shoot(sf::Vector2f startPosition, sf::Vector2f direction);
};

#endif