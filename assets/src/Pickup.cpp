#include "Pickup.h"
#include "Game.h"

#include <cmath>

Pickup::Pickup(sf::Texture& texture, const sf::Vector2f& pos, int val)
{
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setScale(1.35f, 1.35f);
    sprite.setPosition(pos);

    value = val;
    animationTimer = 0.f;
}

void Pickup::update(Game&, float dt)
{
    animationTimer += dt;
    sprite.move(0.f, std::sin(animationTimer * 4.f) * 0.12f);
}

void Pickup::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Pickup::getPosition() const
{
    return sprite.getPosition();
}

float Pickup::getRadius() const
{
    return 18.f;
}

MaterialPickup::MaterialPickup(sf::Texture& t, const sf::Vector2f& p)
    : Pickup(t, p, 1)
{
}

void MaterialPickup::collect(Game& game)
{
    Player* p = game.getPlayer();

    if (p)
    {
        p->addMaterials(value);
        game.addFloatingText("+1 material", getPosition(), sf::Color(80, 220, 100));
    }

    destroy();
}

HealthPickup::HealthPickup(sf::Texture& t, const sf::Vector2f& p)
    : Pickup(t, p, 2)
{
}

void HealthPickup::collect(Game& game)
{
    Player* p = game.getPlayer();

    if (p)
    {
        p->heal(value);
        game.addFloatingText("HP +2", getPosition(), sf::Color(255, 120, 150));
    }

    destroy();
}
