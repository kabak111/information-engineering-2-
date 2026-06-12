#include "Projectile.h"
#include "Game.h"

Projectile::Projectile(
    sf::Texture& texture,
    const sf::Vector2f& pos,
    const sf::Vector2f& vel,
    float dmg,
    bool playerShot
)
{
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setScale(1.2f, 1.2f);
    sprite.setPosition(pos);

    velocity = vel;
    damage = dmg;
    fromPlayer = playerShot;

    if (!fromPlayer)
        sprite.setColor(sf::Color(255, 120, 120));
}

void Projectile::update(Game& game, float dt)
{
    sprite.move(velocity * dt);

    if (!game.isInsideWorld(sprite.getPosition()))
        destroy();
}

void Projectile::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Projectile::getPosition() const
{
    return sprite.getPosition();
}

float Projectile::getRadius() const
{
    return 9.f;
}

float Projectile::getDamage() const
{
    return damage;
}

bool Projectile::isFromPlayer() const
{
    return fromPlayer;
}
