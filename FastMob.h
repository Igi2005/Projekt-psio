#ifndef FASTMOB_H
#define FASTMOB_H

#include "Mob.h"

class FastMob : public Mob
{
public:
    FastMob(sf::Vector2f startPosition);

    void update(float dt, sf::Vector2f playerPosition) override;
    int getPoints() const override;
};

#endif