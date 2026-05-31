#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Player;

class UIManager
{
public:
    UIManager();

    void setFont(sf::Font& font);

    void drawHUD(
        sf::RenderWindow& window,
        const Player& player,
        int wave,
        float waveTimer,
        float gameTime,
        const std::string& lastUpgrade
    );

    void drawShop(sf::RenderWindow& window, const Player& player);
    void drawPause(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int wave, int materials);

private:
    sf::Font* font;

    sf::Text text(
        unsigned size,
        const std::string& s,
        float x,
        float y,
        sf::Color c = sf::Color::Black
    );

    void card(
        sf::RenderWindow& window,
        const std::string& title,
        const std::string& value,
        float x,
        float y,
        float w
    );
};
