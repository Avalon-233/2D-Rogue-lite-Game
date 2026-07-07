#pragma once
#pragma once

#include "SFML/Graphics.hpp"

class Player;
class Enemy;

class StatusBar
{
public:
    StatusBar();

    void SetSize(float width, float height);
    void SetPosition(float x, float y);
    void SetColors(sf::Color backgroundColor, sf::Color fillColor);
    void SetValue(float currentValue, float maxValue);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape _background;
    sf::RectangleShape _fill;

    float _width = 100.f;
    float _height = 10.f;
    float _ratio = 1.f;
};

class UIManager
{
public:
    UIManager();

    void DrawPlayerHUD(sf::RenderWindow& window, const Player* player);
    void DrawEnemyHealthBar(sf::RenderWindow& window, const Enemy* enemy);

private:
    StatusBar _playerHealthBar;
    StatusBar _playerExperienceBar;
    StatusBar _enemyHealthBar;
};
