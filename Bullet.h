#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet
{
private:
    sf::CircleShape shape;

    sf::Vector2f startPosition;
    sf::Vector2f direction;

    float speed;
    float maxDistance;
    int damage;
    bool active;

public:
    Bullet(sf::Vector2f startPosition, sf::Vector2f direction);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    int getDamage() const;

    bool isActive() const;
    void deactivate();
};

#endif