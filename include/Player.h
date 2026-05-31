#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player(sf::Texture& texture, const sf::Vector2f& startPosition, float startSpeed, int startHp);
    void update(Game& game, float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f getPosition() const override;
    float getRadius() const override;
    void damagePlayer(int amount);
    void heal(int amount);
    void addMaterials(int amount);
    int getHp() const; int getMaxHp() const; int getMaterials() const;
    float getDamage() const; float getCooldown() const; float getSpeed() const;
    void upgradeDamage(float amount);
    void upgradeAttackSpeed(float reduction);
    void upgradeMaxHp(int amount);
    void upgradeSpeed(float amount);
private:
    sf::Sprite sprite;
    float speed, damage, attackCooldown, attackTimer, animationTimer;
    int hp, maxHp, materials;
};
