#ifndef MOB_H
#define MOB_H

#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Mob
{
protected:
    sf::Texture texture;
    sf::Sprite sprite;

    float speed;
    int hp;
    int damage;
    bool alive;

    float attackTimer;
    float attackCooldown;

public:
    Mob(sf::Vector2f startPosition, const std::string& texturePath);
    virtual ~Mob();

    virtual void update(float dt, sf::Vector2f playerPosition);
    virtual void draw(sf::RenderWindow& window);

    void takeDamage(int value);
    void attack(Player& player, float dt);

    bool isAlive() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    virtual int getPoints() const;
};

#endif