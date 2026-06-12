#pragma once
#include <SFML/Graphics.hpp>
float length(const sf::Vector2f& v);
sf::Vector2f normalize(const sf::Vector2f& v);
float distance(const sf::Vector2f& a, const sf::Vector2f& b);
float randomFloat(float minValue, float maxValue);
int randomInt(int minValue, int maxValue);
