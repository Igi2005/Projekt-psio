#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>

class Map
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Map();

    void draw(sf::RenderWindow& window);
    sf::Vector2f getSize() const;
};

#endif