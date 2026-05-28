//
// Created by Wetson on 26.05.2026.
//

#ifndef SFML_PROJECT_MAP_H
#define SFML_PROJECT_MAP_H

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

#endif //SFML_PROJECT_MAP_H