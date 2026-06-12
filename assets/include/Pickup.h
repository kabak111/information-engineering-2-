#pragma once

#include "GameObject.h"

class Pickup : public GameObject
{
public:
    Pickup(sf::Texture& texture, const sf::Vector2f& position, int value);

    void update(Game& game, float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    sf::Vector2f getPosition() const override;
    float getRadius() const override;

    virtual void collect(Game& game) = 0;

protected:
    sf::Sprite sprite;
    int value;
    float animationTimer;
};

class MaterialPickup : public Pickup
{
public:
    MaterialPickup(sf::Texture& texture, const sf::Vector2f& position);
    void collect(Game& game) override;
};

class HealthPickup : public Pickup
{
public:
    HealthPickup(sf::Texture& texture, const sf::Vector2f& position);
    void collect(Game& game) override;
};
