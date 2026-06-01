#ifndef TANKMOB_H
#define TANKMOB_H

#include "Mob.h"

class TankMob : public Mob
{
public:
    TankMob(sf::Vector2f startPosition);

    void update(float dt, sf::Vector2f playerPosition) override;
    int getPoints() const override;
};

#endif