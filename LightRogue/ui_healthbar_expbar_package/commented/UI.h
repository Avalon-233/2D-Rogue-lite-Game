#pragma once

#include "SFML/Graphics.hpp"

class Player;
class Enemy;

// StatusBar is a reusable progress bar.
// It can draw health bars, experience bars, cooldown bars, or any other
// "current value / max value" UI element.
class StatusBar
{
public:
    StatusBar();

    // Set the total size of the bar.
    // Player HUD bars are usually wider; enemy overhead bars are smaller.
    void SetSize(float width, float height);

    // Set the bar position.
    // Player HUD bars use fixed screen coordinates.
    // Enemy bars use world coordinates above each enemy.
    void SetPosition(float x, float y);

    // Set background and fill colors.
    // The background is the empty part of the bar.
    // The fill is the current value part of the bar.
    void SetColors(sf::Color backgroundColor, sf::Color fillColor);

    // Set current and maximum values.
    // Example: currentValue = 50, maxValue = 100 means 50%.
    void SetValue(float currentValue, float maxValue);

    // Draw background first, then fill.
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape _background;
    sf::RectangleShape _fill;

    float _width = 100.f;
    float _height = 10.f;
    float _ratio = 1.f;
};

// UIManager owns and draws the first-stage UI:
// player health bar, player experience bar, and enemy health bars.
class UIManager
{
public:
    UIManager();

    // Draw screen-space HUD elements for the player.
    void DrawPlayerHUD(sf::RenderWindow& window, const Player* player);

    // Draw one world-space health bar above an enemy.
    void DrawEnemyHealthBar(sf::RenderWindow& window, const Enemy* enemy);

private:
    StatusBar _playerHealthBar;
    StatusBar _playerExperienceBar;
    StatusBar _enemyHealthBar;
};
