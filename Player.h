//
// Created by Wetson on 16.05.2026.
//

#ifndef SFML_PROJECT_PLAYER_H
#define SFML_PROJECT_PLAYER_H
#include <SFML/Graphics.hpp>
class Player
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    float speed;
    int hp;

public:
    Player(sf::Vector2f startPosition);
    void update(float dt, sf::Vector2f mapSize);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    int getHp() const;
};



#endif //SFML_PROJECT_PLAYER_H