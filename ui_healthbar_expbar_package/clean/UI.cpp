#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"

StatusBar::StatusBar()
{
    _width = 100.f;
    _height = 10.f;

    _background.setSize({ _width, _height });
    _fill.setSize({ _width, _height });

    _background.setFillColor(sf::Color(40, 40, 40));
    _fill.setFillColor(sf::Color::Green);
}

void StatusBar::SetSize(float width, float height)
{
    _width = width;
    _height = height;

    _background.setSize({ _width, _height });
    _fill.setSize({ _width * _ratio, _height });
}

void StatusBar::SetPosition(float x, float y)
{
    _background.setPosition({ x, y });
    _fill.setPosition({ x, y });
}

void StatusBar::SetColors(sf::Color backgroundColor, sf::Color fillColor)
{
    _background.setFillColor(backgroundColor);
    _fill.setFillColor(fillColor);
}

void StatusBar::SetValue(float currentValue, float maxValue)
{
    if (maxValue <= 0.f)
    {
        _ratio = 0.f;
    }
    else
    {
        _ratio = currentValue / maxValue;
    }

    if (_ratio < 0.f)
    {
        _ratio = 0.f;
    }
    if (_ratio > 1.f)
    {
        _ratio = 1.f;
    }

    _fill.setSize({ _width * _ratio, _height });
}

void StatusBar::Draw(sf::RenderWindow& window)
{
    window.draw(_background);
    window.draw(_fill);
}

UIManager::UIManager()
{
    _playerHealthBar.SetSize(220.f, 18.f);
    _playerHealthBar.SetColors(
        sf::Color(40, 40, 40),
        sf::Color(220, 50, 50)
    );

    _playerExperienceBar.SetSize(220.f, 12.f);
    _playerExperienceBar.SetColors(
        sf::Color(40, 40, 40),
        sf::Color(80, 150, 255)
    );

    _enemyHealthBar.SetSize(42.f, 6.f);
    _enemyHealthBar.SetColors(
        sf::Color(30, 30, 30),
        sf::Color(230, 60, 60)
    );
}

void UIManager::DrawPlayerHUD(sf::RenderWindow& window, const Player* player)
{
    if (!player)
    {
        return;
    }

    _playerHealthBar.SetPosition(20.f, 20.f);
    _playerHealthBar.SetValue(player->GetHP(), player->GetMaxHP());
    _playerHealthBar.Draw(window);

    _playerExperienceBar.SetPosition(20.f, 44.f);
    _playerExperienceBar.SetValue(player->GetExperience(), player->GetExperienceNeeded());
    _playerExperienceBar.Draw(window);
}

void UIManager::DrawEnemyHealthBar(sf::RenderWindow& window, const Enemy* enemy)
{
    if (!enemy)
    {
        return;
    }

    if (!enemy->IsExisting())
    {
        return;
    }

    sf::Vector2f enemyPosition = enemy->GetPosition();

    const float barWidth = 42.f;
    const float barHeight = 6.f;
    const float x = enemyPosition.x - barWidth / 2.f;
    const float y = enemyPosition.y - 30.f;

    _enemyHealthBar.SetSize(barWidth, barHeight);
    _enemyHealthBar.SetPosition(x, y);
    _enemyHealthBar.SetValue(enemy->GetHP(), enemy->GetMaxHP());
    _enemyHealthBar.Draw(window);
}
