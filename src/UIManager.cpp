#include "UIManager.h"
#include "Player.h"

#include <sstream>
#include <iomanip>

UIManager::UIManager()
{
    font = nullptr;
}

void UIManager::setFont(sf::Font& f)
{
    font = &f;
}

sf::Text UIManager::text(unsigned size, const std::string& s, float x, float y, sf::Color c)
{
    sf::Text t;
    t.setFont(*font);
    t.setCharacterSize(size);
    t.setString(s);
    t.setFillColor(c);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(2.f);
    t.setPosition(x, y);
    return t;
}

static void centerText(sf::Text& t, float centerX, float y)
{
    sf::FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    t.setPosition(centerX, y);
}

void UIManager::card(sf::RenderWindow& w, const std::string& title, const std::string& value, float x, float y, float cw)
{
    sf::RectangleShape shadow(sf::Vector2f(cw, 52));
    shadow.setPosition(x + 4, y + 4);
    shadow.setFillColor(sf::Color(0, 0, 0, 80));
    w.draw(shadow);

    sf::RectangleShape r(sf::Vector2f(cw, 52));
    r.setPosition(x, y);
    r.setFillColor(sf::Color(248, 244, 232));
    r.setOutlineThickness(3);
    r.setOutlineColor(sf::Color(35, 35, 35));
    w.draw(r);

    sf::Text titleText = text(13, title, x + 10, y + 7, sf::Color(70, 70, 70));
    titleText.setOutlineThickness(0.f);
    w.draw(titleText);

    sf::Text valueText = text(22, value, x + 10, y + 25, sf::Color::White);
    valueText.setStyle(sf::Text::Bold);
    valueText.setOutlineThickness(2.5f);
    w.draw(valueText);
}

void UIManager::drawHUD(sf::RenderWindow& w, const Player& p, int wave, float waveTimer, float gameTime, const std::string& last)
{
    float W = w.getSize().x;
    float H = w.getSize().y;

    float cw = 130;
    float g = 12;
    float totalWidth = 6 * cw + 5 * g;
    float start = W / 2.f - totalWidth / 2.f;
    float y = 12;

    std::ostringstream hp, mat, wv, tm, dmg, cd;

    hp << p.getHp() << "/" << p.getMaxHp();
    mat << p.getMaterials();
    wv << wave;

    int sec = (int)gameTime % 60;
    tm << (int)(gameTime / 60) << ":" << (sec < 10 ? "0" : "") << sec;

    dmg << std::fixed << std::setprecision(1) << p.getDamage();
    cd << std::fixed << std::setprecision(2) << p.getCooldown();

    card(w, "HP", hp.str(), start, y, cw);
    card(w, "MATERIALS", mat.str(), start + 1 * (cw + g), y, cw);
    card(w, "WAVE", wv.str(), start + 2 * (cw + g), y, cw);
    card(w, "TIME", tm.str(), start + 3 * (cw + g), y, cw);
    card(w, "DAMAGE", dmg.str(), start + 4 * (cw + g), y, cw);
    card(w, "COOLDOWN", cd.str(), start + 5 * (cw + g), y, cw);

    sf::RectangleShape hpBack(sf::Vector2f(totalWidth, 15));
    hpBack.setPosition(start, 75);
    hpBack.setFillColor(sf::Color(45, 45, 45));
    hpBack.setOutlineThickness(2);
    hpBack.setOutlineColor(sf::Color::Black);
    w.draw(hpBack);

    float hpPercent = (float)p.getHp() / (float)p.getMaxHp();

    if (hpPercent < 0.f)
        hpPercent = 0.f;

    sf::RectangleShape hpBar(sf::Vector2f(totalWidth * hpPercent, 15));
    hpBar.setPosition(start, 75);
    hpBar.setFillColor(sf::Color(220, 55, 55));
    w.draw(hpBar);

    sf::RectangleShape bottomShadow(sf::Vector2f(W - 30, 48));
    bottomShadow.setPosition(19, H - 52);
    bottomShadow.setFillColor(sf::Color(0, 0, 0, 80));
    w.draw(bottomShadow);

    sf::RectangleShape bottom(sf::Vector2f(W - 30, 48));
    bottom.setPosition(15, H - 56);
    bottom.setFillColor(sf::Color(248, 244, 232));
    bottom.setOutlineThickness(3);
    bottom.setOutlineColor(sf::Color(35, 35, 35));
    w.draw(bottom);

    sf::Text controls = text(21, "WASD / Arrows - Move", 45, H - 34, sf::Color::White);
    controls.setStyle(sf::Text::Bold);
    centerText(controls, 170, H - 32);
    w.draw(controls);

    sf::Text upgrade = text(21, "Last upgrade: " + last, 0, 0, sf::Color(120, 190, 255));
    upgrade.setStyle(sf::Text::Bold);
    centerText(upgrade, W / 2.f, H - 32);
    w.draw(upgrade);

    sf::Text pause = text(21, "ESC - Pause", 0, 0, sf::Color::White);
    pause.setStyle(sf::Text::Bold);
    centerText(pause, W - 140, H - 32);
    w.draw(pause);
}

void UIManager::drawShop(sf::RenderWindow& w, const Player& p)
{
    float W = w.getSize().x;
    float H = w.getSize().y;
    float cx = W / 2.f;

    sf::RectangleShape overlay(sf::Vector2f(W, H));
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    w.draw(overlay);

    sf::RectangleShape shadow(sf::Vector2f(920, 390));
    shadow.setPosition(cx - 460 + 8, H / 2 - 195 + 8);
    shadow.setFillColor(sf::Color(0, 0, 0, 100));
    w.draw(shadow);

    sf::RectangleShape box(sf::Vector2f(920, 390));
    box.setPosition(cx - 460, H / 2 - 195);
    box.setFillColor(sf::Color(248, 244, 232));
    box.setOutlineThickness(4);
    box.setOutlineColor(sf::Color(35, 35, 35));
    w.draw(box);

    float y = box.getPosition().y + 50;

    sf::Text title = text(42, "Choose upgrade between waves", 0, 0, sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setOutlineThickness(3.f);
    centerText(title, cx, y);
    w.draw(title);

    std::ostringstream s1, s2, s3, s4;

    s1 << "1) Damage: " << std::fixed << std::setprecision(1)
       << p.getDamage() << " -> " << p.getDamage() + 0.5f;

    s2 << "2) Cooldown: " << std::fixed << std::setprecision(2)
       << p.getCooldown() << " -> " << p.getCooldown() - 0.04f;

    s3 << "3) Max HP: " << p.getMaxHp()
       << " -> " << p.getMaxHp() + 2 << " and heal +2";

    s4 << "4) Move speed: " << (int)p.getSpeed()
       << " -> " << (int)(p.getSpeed() + 28);

    sf::Text option1 = text(30, s1.str(), 0, 0, sf::Color(255, 210, 80));
    sf::Text option2 = text(30, s2.str(), 0, 0, sf::Color(100, 210, 255));
    sf::Text option3 = text(30, s3.str(), 0, 0, sf::Color(255, 120, 150));
    sf::Text option4 = text(30, s4.str(), 0, 0, sf::Color(110, 235, 130));

    option1.setStyle(sf::Text::Bold);
    option2.setStyle(sf::Text::Bold);
    option3.setStyle(sf::Text::Bold);
    option4.setStyle(sf::Text::Bold);

    centerText(option1, cx, y + 90);
    centerText(option2, cx, y + 145);
    centerText(option3, cx, y + 200);
    centerText(option4, cx, y + 255);

    w.draw(option1);
    w.draw(option2);
    w.draw(option3);
    w.draw(option4);
}

void UIManager::drawPause(sf::RenderWindow& w)
{
    float W = w.getSize().x;
    float H = w.getSize().y;
    float cx = W / 2.f;
    float cy = H / 2.f;

    sf::RectangleShape overlay(sf::Vector2f(W, H));
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    w.draw(overlay);

    sf::RectangleShape box(sf::Vector2f(520, 210));
    box.setPosition(cx - 260, cy - 105);
    box.setFillColor(sf::Color(248, 244, 232));
    box.setOutlineThickness(4);
    box.setOutlineColor(sf::Color(35, 35, 35));
    w.draw(box);

    sf::Text title = text(58, "PAUSED", 0, 0, sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setOutlineThickness(4.f);
    centerText(title, cx, cy - 45);
    w.draw(title);

    sf::Text info = text(30, "Press ESC to continue", 0, 0, sf::Color::White);
    info.setStyle(sf::Text::Bold);
    info.setOutlineThickness(3.f);
    centerText(info, cx, cy + 45);
    w.draw(info);
}

void UIManager::drawGameOver(sf::RenderWindow& w, int wave, int mat)
{
    float W = w.getSize().x;
    float H = w.getSize().y;
    float cx = W / 2.f;
    float cy = H / 2.f;

    sf::RectangleShape overlay(sf::Vector2f(W, H));
    overlay.setFillColor(sf::Color(0, 0, 0, 185));
    w.draw(overlay);

    sf::RectangleShape shadow(sf::Vector2f(760, 330));
    shadow.setPosition(cx - 380 + 8, cy - 165 + 8);
    shadow.setFillColor(sf::Color(0, 0, 0, 110));
    w.draw(shadow);

    sf::RectangleShape box(sf::Vector2f(760, 330));
    box.setPosition(cx - 380, cy - 165);
    box.setFillColor(sf::Color(248, 244, 232));
    box.setOutlineThickness(4);
    box.setOutlineColor(sf::Color(35, 35, 35));
    w.draw(box);

    std::ostringstream result;
    result << "You survived " << wave << " waves and collected " << mat << " materials";

    sf::Text title = text(62, "GAME OVER", 0, 0, sf::Color(255, 100, 100));
    title.setStyle(sf::Text::Bold);
    title.setOutlineThickness(4.f);
    centerText(title, cx, cy - 105);
    w.draw(title);

    sf::Text info = text(27, result.str(), 0, 0, sf::Color::White);
    info.setStyle(sf::Text::Bold);
    info.setOutlineThickness(3.f);
    centerText(info, cx, cy - 25);
    w.draw(info);

    sf::Text restart = text(30, "Press R to Restart", 0, 0, sf::Color(120, 255, 140));
    restart.setStyle(sf::Text::Bold);
    restart.setOutlineThickness(3.f);
    centerText(restart, cx, cy + 55);
    w.draw(restart);

    sf::Text quit = text(28, "Press Q to Quit", 0, 0, sf::Color(120, 190, 255));
    quit.setStyle(sf::Text::Bold);
    quit.setOutlineThickness(3.f);
    centerText(quit, cx, cy + 105);
    w.draw(quit);
}