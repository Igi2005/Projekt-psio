#ifndef MOB_H
#define MOB_H

#include <SFML/Graphics.hpp>
#include <string>

class Mob
{
protected:
    sf::Texture texture;
    sf::Sprite sprite;

    float speed;
    int hp;
    int damage;
    bool alive;

public:
    Mob(sf::Vector2f startPosition, const std::string& texturePath);
    virtual ~Mob();

    virtual void update(float dt, sf::Vector2f playerPosition);
    virtual void draw(sf::RenderWindow& window);

    void takeDamage(int value);

    bool isAlive() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    virtual int getPoints() const;
};

#endif