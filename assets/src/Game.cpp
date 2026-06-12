#include "Game.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"
#include "MathUtils.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

Game::Game()
{
    std::srand((unsigned)std::time(NULL));

    sf::VideoMode mode(1200, 800);

    window.create(
        mode,
        "Brotato OOP Implementation",
        sf::Style::Titlebar | sf::Style::Close
    );

    window.setFramerateLimit(60);

    arena = sf::FloatRect(
        10.f,
        90.f,
        (float)mode.width - 20.f,
        (float)mode.height - 160.f
    );

    player = nullptr;
    state = Playing;
    wave = 1;
    waveTimer = config.waveDuration;
    spawnTimer = 0.f;
    gameTime = 0.f;
    lastUpgrade = "None";

    loadResources();
    ui.setFont(resources.font("main"));
    reset();
}

void Game::loadResources()
{
    resources.loadTexture("player", "assets/player.png");
    resources.loadTexture("melee", "assets/enemy_melee.png");
    resources.loadTexture("fast", "assets/enemy_fast.png");
    resources.loadTexture("ranged", "assets/enemy_ranged.png");
    resources.loadTexture("material", "assets/material.png");
    resources.loadTexture("heart", "assets/heart.png");
    resources.loadTexture("bullet", "assets/bullet.png");
    resources.loadTexture("rock", "assets/rock.png");
    resources.loadTexture("grass", "assets/grass.png");
    resources.loadFont("main", "assets/font.ttf");
}

void Game::reset()
{
    objects.clear();
    floatingTexts.clear();
    decorations.clear();

    sf::Vector2f start(window.getSize().x / 2.f, window.getSize().y / 2.f);

    auto p = std::make_unique<Player>(
        resources.texture("player"),
        start,
        config.playerStartSpeed,
        config.playerStartHp
    );

    player = p.get();
    objects.push_back(std::move(p));

    for (int i = 0; i < 45; i++)
    {
        sf::Sprite rock(resources.texture("rock"));
        rock.setOrigin(24, 24);
        rock.setPosition(
            randomFloat(arena.left + 30, arena.left + arena.width - 30),
            randomFloat(arena.top + 30, arena.top + arena.height - 30)
        );
        rock.setScale(randomFloat(.7f, 1.3f), randomFloat(.7f, 1.3f));
        decorations.push_back(rock);

        sf::Sprite grass(resources.texture("grass"));
        grass.setOrigin(24, 24);
        grass.setPosition(
            randomFloat(arena.left + 30, arena.left + arena.width - 30),
            randomFloat(arena.top + 30, arena.top + arena.height - 30)
        );
        grass.setScale(randomFloat(.6f, 1.2f), randomFloat(.6f, 1.2f));
        decorations.push_back(grass);
    }

    state = Playing;
    wave = 1;
    waveTimer = config.waveDuration;
    spawnTimer = 0;
    gameTime = 0;
    lastUpgrade = "None";
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        if (dt > .04f)
            dt = .04f;

        handleEvents();
        update(dt);
        render();
    }
}

void Game::handleEvents()
{
    sf::Event e;

    while (window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
            window.close();

        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Escape)
            {
                if (state == Playing)
                    state = Paused;
                else if (state == Paused)
                    state = Playing;
            }

            if (e.key.code == sf::Keyboard::R && state == GameOver)
                reset();

            if (e.key.code == sf::Keyboard::Q && state == GameOver)
                window.close();

            if (state == Shop)
            {
                if (e.key.code == sf::Keyboard::Num1)
                    applyUpgrade(1);

                if (e.key.code == sf::Keyboard::Num2)
                    applyUpgrade(2);

                if (e.key.code == sf::Keyboard::Num3)
                    applyUpgrade(3);

                if (e.key.code == sf::Keyboard::Num4)
                    applyUpgrade(4);
            }
        }
    }
}

void Game::update(float dt)
{
    updateFloatingTexts(dt);

    if (state != Playing)
        return;

    gameTime += dt;
    waveTimer -= dt;
    spawnTimer -= dt;

    if (spawnTimer <= 0)
    {
        spawnEnemy();
        spawnTimer = std::max(.22f, config.baseSpawnInterval - wave * .05f);
    }

    std::size_t objectCount = objects.size();

    for (std::size_t i = 0; i < objectCount; i++)
    {
        if (i < objects.size() && objects[i]->isAlive())
            objects[i]->update(*this, dt);
    }

    handleCollisions();
    removeDeadObjects();

    if (player && !player->isAlive())
        state = GameOver;

    if (waveTimer <= 0 && player && player->isAlive())
    {
        wave++;
        waveTimer = config.waveDuration;
        state = Shop;
        addFloatingText("Wave cleared!", player->getPosition(), sf::Color::White);
    }
}

void Game::render()
{
    window.clear(sf::Color(238, 233, 220));

    sf::RectangleShape bg(sf::Vector2f(arena.width, arena.height));
    bg.setPosition(arena.left, arena.top);
    bg.setFillColor(sf::Color(245, 240, 230));
    bg.setOutlineThickness(2);
    bg.setOutlineColor(sf::Color(40, 40, 40));
    window.draw(bg);

    drawDecorations();

    for (auto& o : objects)
        o->draw(window);

    for (auto& f : floatingTexts)
        window.draw(f.text);

    if (player)
        ui.drawHUD(window, *player, wave, waveTimer, gameTime, lastUpgrade);

    if (state == Paused)
        ui.drawPause(window);
    else if (state == Shop && player)
        ui.drawShop(window, *player);
    else if (state == GameOver && player)
        ui.drawGameOver(window, wave, player->getMaterials());

    window.display();
}

void Game::spawnEnemy()
{
    sf::Vector2f p = randomSpawnPosition();
    int roll = randomInt(0, 99);

    if (wave < 3)
    {
        if (roll < 75)
            objects.push_back(std::make_unique<MeleeEnemy>(resources.texture("melee"), p, wave));
        else
            objects.push_back(std::make_unique<FastEnemy>(resources.texture("fast"), p, wave));
    }
    else
    {
        if (roll < 55)
            objects.push_back(std::make_unique<MeleeEnemy>(resources.texture("melee"), p, wave));
        else if (roll < 82)
            objects.push_back(std::make_unique<FastEnemy>(resources.texture("fast"), p, wave));
        else
            objects.push_back(std::make_unique<RangedEnemy>(resources.texture("ranged"), p, wave));
    }
}

sf::Vector2f Game::randomSpawnPosition() const
{
    int s = randomInt(0, 3);

    if (s == 0)
    {
        return sf::Vector2f(
            randomFloat(arena.left, arena.left + arena.width),
            arena.top - 40
        );
    }

    if (s == 1)
    {
        return sf::Vector2f(
            randomFloat(arena.left, arena.left + arena.width),
            arena.top + arena.height + 40
        );
    }

    if (s == 2)
    {
        return sf::Vector2f(
            arena.left - 40,
            randomFloat(arena.top, arena.top + arena.height)
        );
    }

    return sf::Vector2f(
        arena.left + arena.width + 40,
        randomFloat(arena.top, arena.top + arena.height)
    );
}

void Game::handleCollisions()
{
    if (!player)
        return;

    std::vector<sf::Vector2f> pickupPositions;

    for (auto& a : objects)
    {
        Projectile* pr = dynamic_cast<Projectile*>(a.get());

        if (!pr || !pr->isAlive())
            continue;

        if (pr->isFromPlayer())
        {
            for (auto& b : objects)
            {
                Enemy* en = dynamic_cast<Enemy*>(b.get());

                if (!en || !en->isAlive())
                    continue;

                if (distance(pr->getPosition(), en->getPosition()) < pr->getRadius() + en->getRadius())
                {
                    en->takeDamage(pr->getDamage());
                    pr->destroy();

                    std::ostringstream ss;
                    ss << "-" << std::fixed << std::setprecision(1) << pr->getDamage();

                    addFloatingText(ss.str(), en->getPosition(), sf::Color(255, 235, 100));

                    if (!en->isAlive())
                        pickupPositions.push_back(en->getPosition());

                    break;
                }
            }
        }
        else
        {
            if (distance(pr->getPosition(), player->getPosition()) < pr->getRadius() + player->getRadius())
            {
                player->damagePlayer((int)pr->getDamage());
                pr->destroy();
                addFloatingText("-1 HP", player->getPosition(), sf::Color(255, 80, 80));
            }
        }
    }

    for (auto& o : objects)
    {
        Enemy* en = dynamic_cast<Enemy*>(o.get());

        if (en && en->isAlive())
        {
            if (distance(en->getPosition(), player->getPosition()) < en->getRadius() + player->getRadius())
            {
                player->damagePlayer(en->getContactDamage());
                en->destroy();
                addFloatingText("-1 HP", player->getPosition(), sf::Color(255, 80, 80));
            }
        }

        Pickup* pu = dynamic_cast<Pickup*>(o.get());

        if (pu && pu->isAlive())
        {
            if (distance(pu->getPosition(), player->getPosition()) < pu->getRadius() + player->getRadius())
                pu->collect(*this);
        }
    }

    for (const auto& pos : pickupPositions)
        createPickup(pos);
}

void Game::removeDeadObjects()
{
    objects.erase(
        std::remove_if(
            objects.begin(),
            objects.end(),
            [](const std::unique_ptr<GameObject>& o)
            {
                Player* p = dynamic_cast<Player*>(o.get());

                if (p)
                    return false;

                return !o->isAlive();
            }
        ),
        objects.end()
    );

    player = nullptr;

    for (auto& o : objects)
    {
        Player* p = dynamic_cast<Player*>(o.get());

        if (p)
        {
            player = p;
            break;
        }
    }
}

void Game::applyUpgrade(int option)
{
    if (!player)
        return;

    if (option == 1)
    {
        player->upgradeDamage(.5f);
        lastUpgrade = "+0.5 Damage";
    }
    else if (option == 2)
    {
        player->upgradeAttackSpeed(.04f);
        lastUpgrade = "-0.04 Cooldown";
    }
    else if (option == 3)
    {
        player->upgradeMaxHp(2);
        lastUpgrade = "+2 Max HP";
    }
    else if (option == 4)
    {
        player->upgradeSpeed(28.f);
        lastUpgrade = "+28 Move Speed";
    }

    state = Playing;
}

void Game::createPickup(const sf::Vector2f& p)
{
    objects.push_back(std::make_unique<MaterialPickup>(resources.texture("material"), p));

    if (randomFloat(0, 1) < .12f)
    {
        objects.push_back(
            std::make_unique<HealthPickup>(
                resources.texture("heart"),
                p + sf::Vector2f(20, 8)
            )
        );
    }
}

void Game::updateFloatingTexts(float dt)
{
    for (auto& f : floatingTexts)
    {
        f.lifeTime -= dt;
        f.text.move(f.velocity * dt);

        sf::Color c = f.text.getFillColor();
        c.a = (sf::Uint8)(std::max(0.f, f.lifeTime / 1.1f) * 255.f);
        f.text.setFillColor(c);
    }

    floatingTexts.erase(
        std::remove_if(
            floatingTexts.begin(),
            floatingTexts.end(),
            [](const FloatingText& f)
            {
                return f.lifeTime <= 0;
            }
        ),
        floatingTexts.end()
    );
}

void Game::drawDecorations()
{
    for (auto& d : decorations)
        window.draw(d);
}

Player* Game::getPlayer()
{
    return player;
}

void Game::keepInsideArena(sf::Sprite& s)
{
    sf::Vector2f p = s.getPosition();
    float m = 30;

    if (p.x < arena.left + m)
        p.x = arena.left + m;

    if (p.x > arena.left + arena.width - m)
        p.x = arena.left + arena.width - m;

    if (p.y < arena.top + m)
        p.y = arena.top + m;

    if (p.y > arena.top + arena.height - m)
        p.y = arena.top + arena.height - m;

    s.setPosition(p);
}

bool Game::isInsideWorld(const sf::Vector2f& p) const
{
    return p.x > -80 &&
           p.y > -80 &&
           p.x < window.getSize().x + 80 &&
           p.y < window.getSize().y + 80;
}

void Game::playerShoot()
{
    if (!player || !player->isAlive())
        return;

    Enemy* closest = nullptr;
    float best = 9999999;

    for (auto& o : objects)
    {
        Enemy* e = dynamic_cast<Enemy*>(o.get());

        if (e && e->isAlive())
        {
            float d = distance(player->getPosition(), e->getPosition());

            if (d < best)
            {
                best = d;
                closest = e;
            }
        }
    }

    if (!closest)
        return;

    sf::Vector2f dir = normalize(closest->getPosition() - player->getPosition());

    objects.push_back(
        std::make_unique<Projectile>(
            resources.texture("bullet"),
            player->getPosition(),
            dir * 720.f,
            player->getDamage(),
            true
        )
    );
}

void Game::enemyShoot(const sf::Vector2f& pos, const sf::Vector2f& dir)
{
    objects.push_back(
        std::make_unique<Projectile>(
            resources.texture("bullet"),
            pos,
            dir * 300.f,
            1.f,
            false
        )
    );
}

void Game::addFloatingText(const std::string& msg, const sf::Vector2f& pos, sf::Color color)
{
    FloatingText f;

    f.text.setFont(resources.font("main"));
    f.text.setCharacterSize(22);
    f.text.setStyle(sf::Text::Bold);
    f.text.setString(msg);
    f.text.setFillColor(color);
    f.text.setPosition(pos);

    f.velocity = sf::Vector2f(0, -38);
    f.lifeTime = 1.1f;

    floatingTexts.push_back(f);
}
