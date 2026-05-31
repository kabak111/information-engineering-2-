#include "Player.h"
#include "Game.h"

#include <algorithm>
#include <cmath>

Player::Player(sf::Texture& texture, const sf::Vector2f& pos, float startSpeed, int startHp)
{
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(pos);

    speed = startSpeed;
    hp = startHp;
    maxHp = startHp;
    materials = 0;

    damage = 1.f;
    attackCooldown = 0.42f;
    attackTimer = 0.f;
    animationTimer = 0.f;
}

void Player::update(Game& game, float dt)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        movement.y -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        movement.y += speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        movement.x -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        movement.x += speed * dt;

    sprite.move(movement);
    game.keepInsideArena(sprite);

    attackTimer -= dt;

    if (attackTimer <= 0.f)
    {
        game.playerShoot();
        attackTimer = attackCooldown;
    }

    animationTimer += dt;

    float pulse = 2.f + std::sin(animationTimer * 5.f) * 0.05f;
    sprite.setScale(pulse, pulse);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

float Player::getRadius() const
{
    return 25.f;
}

void Player::damagePlayer(int amount)
{
    hp -= amount;

    if (hp <= 0)
    {
        hp = 0;
        destroy();
    }
}

void Player::heal(int amount)
{
    hp = std::min(maxHp, hp + amount);
}

void Player::addMaterials(int amount)
{
    materials += amount;
}

int Player::getHp() const
{
    return hp;
}

int Player::getMaxHp() const
{
    return maxHp;
}

int Player::getMaterials() const
{
    return materials;
}

float Player::getDamage() const
{
    return damage;
}

float Player::getCooldown() const
{
    return attackCooldown;
}

float Player::getSpeed() const
{
    return speed;
}

void Player::upgradeDamage(float amount)
{
    damage += amount;
}

void Player::upgradeAttackSpeed(float reduction)
{
    attackCooldown = std::max(0.12f, attackCooldown - reduction);
}

void Player::upgradeMaxHp(int amount)
{
    maxHp += amount;
    heal(amount);
}

void Player::upgradeSpeed(float amount)
{
    speed += amount;
}
