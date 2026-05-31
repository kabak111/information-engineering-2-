#include "ResourceManager.h"
bool ResourceManager::loadTexture(const std::string& id,const std::string& path){ sf::Texture t; if(!t.loadFromFile(path)) return false; t.setSmooth(true); textures[id]=t; return true; }
bool ResourceManager::loadFont(const std::string& id,const std::string& path){ sf::Font f; if(!f.loadFromFile(path)) return false; fonts[id]=f; return true; }
sf::Texture& ResourceManager::texture(const std::string& id){ return textures.at(id); }
sf::Font& ResourceManager::font(const std::string& id){ return fonts.at(id); }
