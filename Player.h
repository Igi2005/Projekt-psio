#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Font font;
    sf::Text speechText;
    float speechTimer;
    float speed;
    int hp;

public:
    Player(sf::Vector2f startPosition);
    void update(float dt, sf::Vector2f mapSize);
    void draw(sf::RenderWindow& window);
    void say(std::string message);
    sf::Vector2f getPosition() const;
    int getHp() const;
    void takeDamage(int damage);
    sf::FloatRect getBounds() const;
    bool isAlive() const;
};