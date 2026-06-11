#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f startPosition, sf::Vector2f direction)
    : startPosition(startPosition),
      direction(direction),
      speed(700.f),
      maxDistance(600.f),
      damage(25),
      active(true)
{
    shape.setRadius(6.f);
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(6.f, 6.f);
    shape.setPosition(startPosition);
}

void Bullet::update(float dt)
{
    shape.move(direction * speed * dt);

    sf::Vector2f currentPosition = shape.getPosition();

    float dx = currentPosition.x - startPosition.x;
    float dy = currentPosition.y - startPosition.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance >= maxDistance)
    {
        active = false;
    }
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const
{
    return shape.getGlobalBounds();
}

int Bullet::getDamage() const
{
    return damage;
}

bool Bullet::isActive() const
{
    return active;
}

void Bullet::deactivate()
{
    active = false;
}