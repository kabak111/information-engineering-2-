#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "GameConfig.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "Player.h"

struct FloatingText { sf::Text text; sf::Vector2f velocity; float lifeTime; };

// Main supervising class. It controls the window, game loop, events,
// spawning, collisions, waves, shop, pause and game over state.
class Game
{
public:
    Game();
    void run();
    Player* getPlayer();
    void keepInsideArena(sf::Sprite& sprite);
    bool isInsideWorld(const sf::Vector2f& position) const;
    void playerShoot();
    void enemyShoot(const sf::Vector2f& position, const sf::Vector2f& direction);
    void addFloatingText(const std::string& message, const sf::Vector2f& position, sf::Color color);
private:
    enum State { Playing, Paused, Shop, GameOver };
    void loadResources(); void reset(); void handleEvents(); void update(float dt); void render();
    void spawnEnemy(); sf::Vector2f randomSpawnPosition() const; void handleCollisions(); void removeDeadObjects();
    void applyUpgrade(int option); void createPickup(const sf::Vector2f& position); void updateFloatingTexts(float dt); void drawDecorations();
private:
    sf::RenderWindow window; sf::FloatRect arena; GameConfig config; ResourceManager resources; UIManager ui;
    // Required single polymorphic container for all game objects.
    std::vector<std::unique_ptr<GameObject>> objects;
    Player* player;
    std::vector<sf::Sprite> decorations; std::vector<FloatingText> floatingTexts;
    State state; int wave; float waveTimer, spawnTimer, gameTime; std::string lastUpgrade;
};
