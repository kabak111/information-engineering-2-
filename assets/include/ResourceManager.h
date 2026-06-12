#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

// Loads textures and fonts once and stores them by string id.
class ResourceManager
{
public:
    bool loadTexture(const std::string& id, const std::string& path);
    bool loadFont(const std::string& id, const std::string& path);
    sf::Texture& texture(const std::string& id);
    sf::Font& font(const std::string& id);
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
};
