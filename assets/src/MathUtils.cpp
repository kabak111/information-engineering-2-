#include "MathUtils.h"

#include <cmath>
#include <cstdlib>

float length(const sf::Vector2f& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(const sf::Vector2f& v)
{
    float len = length(v);

    if (len == 0.f)
        return sf::Vector2f(0.f, 0.f);

    return sf::Vector2f(v.x / len, v.y / len);
}

float distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return length(a - b);
}

float randomFloat(float minValue, float maxValue)
{
    return minValue + static_cast<float>(std::rand()) / RAND_MAX * (maxValue - minValue);
}

int randomInt(int minValue, int maxValue)
{
    return minValue + std::rand() % (maxValue - minValue + 1);
}
