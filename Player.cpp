#include "Player.h"
#include <iostream>

Player::Player(sf::Vector2f startPosition)
    : speed(250.f), hp(100), speechTimer(0.f)
{
    texture.loadFromFile("player.png");
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(startPosition);
    sprite.setScale(0.2f, 0.2f);

    if (!font.loadFromFile("Roboto_Condensed-Black.ttf")) {
        std::cerr << "Blad: Nie znaleziono Roboto_Condensed-Black.ttf" << std::endl;
    }
    speechText.setFont(font);
    speechText.setCharacterSize(40);
    speechText.setFillColor(sf::Color::Yellow);
    speechText.setOutlineColor(sf::Color::Black);
    speechText.setOutlineThickness(2.f);
}

void Player::update(float dt, sf::Vector2f mapSize)
{
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * dt;
    sprite.move(movement);

    sf::Vector2f pos = sprite.getPosition();
    float hw = sprite.getGlobalBounds().width / 2.f;
    float hh = sprite.getGlobalBounds().height / 2.f;
    if (pos.x < hw) pos.x = hw;
    if (pos.y < hh) pos.y = hh;
    if (pos.x > mapSize.x - hw) pos.x = mapSize.x - hw;
    if (pos.y > mapSize.y - hh) pos.y = mapSize.y - hh;
    sprite.setPosition(pos);

    if (speechTimer > 0) {
        speechTimer -= dt;
        speechText.setPosition(sprite.getPosition().x - 50, sprite.getPosition().y - 100);
    }
}

void Player::say(std::string message) {
    speechText.setString(message);
    speechTimer = 2.0f;
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (speechTimer > 0) window.draw(speechText);
}

sf::Vector2f Player::getPosition() const { return sprite.getPosition(); }
int Player::getHp() const { return hp; }
void Player::takeDamage(int damage) { hp -= damage; if (hp < 0) hp = 0; }
sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }
bool Player::isAlive() const { return hp > 0; }