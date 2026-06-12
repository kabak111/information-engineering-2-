#include "ResourceManager.h"

bool ResourceManager::loadTexture(const std::string& id, const std::string& path)
{
    sf::Texture texture;

    if (!texture.loadFromFile(path))
        return false;

    texture.setSmooth(true);
    textures[id] = texture;

    return true;
}

bool ResourceManager::loadFont(const std::string& id, const std::string& path)
{
    sf::Font loadedFont;

    if (!loadedFont.loadFromFile(path))
        return false;

    fonts[id] = loadedFont;

    return true;
}

sf::Texture& ResourceManager::texture(const std::string& id)
{
    return textures.at(id);
}

sf::Font& ResourceManager::font(const std::string& id)
{
    return fonts.at(id);
}
