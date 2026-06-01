#include "Map.h"
#include <iostream>

Map::Map()
{
    if (!texture.loadFromFile("mapa.png"))
    {
        std::cerr << "Failed to load mapa.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(0.f, 0.f);
}

void Map::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Map::getSize() const
{
    return sf::Vector2f(
        static_cast<float>(texture.getSize().x),
        static_cast<float>(texture.getSize().y)
    );
}