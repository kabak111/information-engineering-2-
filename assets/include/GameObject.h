#pragma once

#include <SFML/Graphics.hpp>

class Game;

class GameObject
{
public:
    virtual ~GameObject() {}

    virtual void update(Game& game, float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual float getRadius() const = 0;

    bool isAlive() const;
    void destroy();

protected:
    bool alive = true;
};
