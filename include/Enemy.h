#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
public:
    Enemy(
        sf::Texture& texture,
        const sf::Vector2f& position,
        float hp,
        float speed,
        int contactDamage
    );

    void update(Game& game, float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    sf::Vector2f getPosition() const override;
    float getRadius() const override;

    void takeDamage(float amount);
    int getContactDamage() const;

protected:
    sf::Sprite sprite;
    float hp;
    float speed;
    float animationTimer;
    int contactDamage;
};

class MeleeEnemy : public Enemy
{
public:
    MeleeEnemy(sf::Texture& texture, const sf::Vector2f& position, int wave);
};

class FastEnemy : public Enemy
{
public:
    FastEnemy(sf::Texture& texture, const sf::Vector2f& position, int wave);
};

class RangedEnemy : public Enemy
{
public:
    RangedEnemy(sf::Texture& texture, const sf::Vector2f& position, int wave);
    void update(Game& game, float deltaTime) override;

private:
    float shootTimer;
};
