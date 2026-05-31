#pragma once
#include "GameObject.h"
class Projectile : public GameObject
{
public:
    Projectile(sf::Texture& texture,const sf::Vector2f& position,const sf::Vector2f& velocity,float damage,bool fromPlayer);
    void update(Game& game,float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f getPosition() const override;
    float getRadius() const override;
    float getDamage() const;
    bool isFromPlayer() const;
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float damage;
    bool fromPlayer;
};
