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
    _fontLoaded =
        _font.openFromFile("SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("LightRogue/SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("../SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("../../SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("C:/Windows/Fonts/segoeui.ttf");

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

void UIManager::DrawUpgradeChoices(sf::RenderWindow& window, const Player* player)
{
    if (!player)
    {
        return;
    }

    const auto& options = player->GetCurrentUpgradeOptions();
    const sf::Vector2f windowSize(window.getSize());

    sf::RectangleShape overlay(windowSize);
    overlay.setFillColor(sf::Color(4, 7, 18, 218));
    window.draw(overlay);

    sf::RectangleShape topRule({ 660.f, 2.f });
    topRule.setPosition({ windowSize.x / 2.f - 330.f, 104.f });
    topRule.setFillColor(sf::Color(68, 214, 221, 150));
    window.draw(topRule);

    DrawText(window, "CHOOSE YOUR LIGHT CODE", 34u, { windowSize.x / 2.f, 58.f }, sf::Color(238, 212, 132), true);
    DrawText(window, "Press 1 / 2 / 3 to install one upgrade", 18u, { windowSize.x / 2.f, 112.f }, sf::Color(178, 226, 229), true);

    const sf::Vector2f cardSize(250.f, 365.f);
    const float gap = 38.f;
    const float totalWidth = cardSize.x * 3.f + gap * 2.f;
    const float startX = windowSize.x / 2.f - totalWidth / 2.f;
    const float cardY = 184.f;

    for (int i = 0; i < 3; ++i)
    {
        DrawUpgradeCard(
            window,
            { startX + static_cast<float>(i) * (cardSize.x + gap), cardY },
            cardSize,
            i + 1,
            options[i]
        );
    }

    sf::RectangleShape bottomRule({ 520.f, 2.f });
    bottomRule.setPosition({ windowSize.x / 2.f - 260.f, 604.f });
    bottomRule.setFillColor(sf::Color(198, 164, 72, 135));
    window.draw(bottomRule);

    DrawText(window, "The battle is paused while this panel is open", 16u, { windowSize.x / 2.f, 632.f }, sf::Color(151, 169, 188), true);
}

void UIManager::DrawUpgradeCard(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, int shortcut, int upgradeType)
{
    const sf::Color cardFill(10, 17, 34, 238);
    const sf::Color cyan(74, 218, 226);
    const sf::Color gold(218, 178, 79);
    const sf::Color muted(142, 161, 184);

    sf::RectangleShape shadow(size);
    shadow.setPosition(position + sf::Vector2f(8.f, 10.f));
    shadow.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(shadow);

    sf::RectangleShape outer(size);
    outer.setPosition(position);
    outer.setFillColor(cardFill);
    outer.setOutlineThickness(3.f);
    outer.setOutlineColor(gold);
    window.draw(outer);

    sf::RectangleShape inner(size - sf::Vector2f(22.f, 22.f));
    inner.setPosition(position + sf::Vector2f(11.f, 11.f));
    inner.setFillColor(sf::Color::Transparent);
    inner.setOutlineThickness(1.f);
    inner.setOutlineColor(sf::Color(74, 218, 226, 160));
    window.draw(inner);

    sf::RectangleShape topGlow({ size.x - 36.f, 4.f });
    topGlow.setPosition(position + sf::Vector2f(18.f, 22.f));
    topGlow.setFillColor(sf::Color(74, 218, 226, 180));
    window.draw(topGlow);

    sf::ConvexShape leftCorner(3);
    leftCorner.setPoint(0, position);
    leftCorner.setPoint(1, position + sf::Vector2f(38.f, 0.f));
    leftCorner.setPoint(2, position + sf::Vector2f(0.f, 38.f));
    leftCorner.setFillColor(sf::Color(218, 178, 79, 210));
    window.draw(leftCorner);

    sf::ConvexShape rightCorner(3);
    rightCorner.setPoint(0, position + sf::Vector2f(size.x, size.y));
    rightCorner.setPoint(1, position + sf::Vector2f(size.x - 38.f, size.y));
    rightCorner.setPoint(2, position + sf::Vector2f(size.x, size.y - 38.f));
    rightCorner.setFillColor(sf::Color(218, 178, 79, 150));
    window.draw(rightCorner);

    sf::CircleShape badge(36.f, 6);
    badge.setOrigin({ 36.f, 36.f });
    badge.setPosition(position + sf::Vector2f(size.x / 2.f, 0.f));
    badge.setRotation(sf::degrees(30.f));
    badge.setFillColor(sf::Color(13, 29, 49));
    badge.setOutlineThickness(3.f);
    badge.setOutlineColor(gold);
    window.draw(badge);

    DrawText(window, std::to_string(shortcut), 30u, position + sf::Vector2f(size.x / 2.f, -19.f), sf::Color(244, 232, 174), true);

    sf::CircleShape iconPlate(68.f, 6);
    iconPlate.setOrigin({ 68.f, 68.f });
    iconPlate.setPosition(position + sf::Vector2f(size.x / 2.f, 126.f));
    iconPlate.setRotation(sf::degrees(30.f));
    iconPlate.setFillColor(sf::Color(7, 35, 52));
    iconPlate.setOutlineThickness(2.f);
    iconPlate.setOutlineColor(sf::Color(74, 218, 226, 190));
    window.draw(iconPlate);

    DrawUpgradeIcon(window, position + sf::Vector2f(size.x / 2.f, 126.f), upgradeType);

    DrawText(window, GetUpgradeName(upgradeType), 23u, position + sf::Vector2f(size.x / 2.f, 218.f), sf::Color(241, 225, 164), true);
    DrawText(window, GetUpgradeDescription(upgradeType), 16u, position + sf::Vector2f(size.x / 2.f, 262.f), sf::Color(190, 215, 220), true);

    sf::RectangleShape chooseBar({ size.x - 68.f, 32.f });
    chooseBar.setPosition(position + sf::Vector2f(34.f, size.y - 62.f));
    chooseBar.setFillColor(sf::Color(21, 43, 62, 230));
    chooseBar.setOutlineThickness(1.f);
    chooseBar.setOutlineColor(sf::Color(218, 178, 79, 170));
    window.draw(chooseBar);

    DrawText(window, "Press " + std::to_string(shortcut), 16u, position + sf::Vector2f(size.x / 2.f, size.y - 55.f), muted, true);
}

void UIManager::DrawUpgradeIcon(sf::RenderWindow& window, sf::Vector2f center, int upgradeType)
{
    const sf::Color cyan(80, 225, 232);
    const sf::Color gold(230, 181, 74);
    const sf::Color red(236, 89, 92);
    const sf::Color green(98, 222, 147);
    const sf::Color blue(105, 164, 255);

    if (upgradeType == 1)
    {
        for (int i = 0; i < 3; ++i)
        {
            sf::CircleShape bullet(9.f);
            bullet.setOrigin({ 9.f, 9.f });
            bullet.setPosition(center + sf::Vector2f(-30.f + i * 30.f, -6.f + std::abs(i - 1) * 16.f));
            bullet.setFillColor(cyan);
            window.draw(bullet);
        }
        return;
    }

    if (upgradeType == 2)
    {
        sf::CircleShape diamond(31.f, 4);
        diamond.setOrigin({ 31.f, 31.f });
        diamond.setPosition(center);
        diamond.setRotation(sf::degrees(45.f));
        diamond.setFillColor(red);
        window.draw(diamond);
        sf::RectangleShape slash({ 58.f, 7.f });
        slash.setOrigin({ 29.f, 3.5f });
        slash.setPosition(center);
        slash.setRotation(sf::degrees(-25.f));
        slash.setFillColor(gold);
        window.draw(slash);
        return;
    }

    if (upgradeType == 3)
    {
        for (int i = 0; i < 4; ++i)
        {
            sf::RectangleShape bolt({ 54.f - i * 7.f, 7.f });
            bolt.setOrigin({ 27.f - i * 3.5f, 3.5f });
            bolt.setPosition(center + sf::Vector2f(0.f, -27.f + i * 18.f));
            bolt.setFillColor(i % 2 == 0 ? cyan : gold);
            window.draw(bolt);
        }
        return;
    }

    if (upgradeType == 4)
    {
        sf::RectangleShape shaft({ 70.f, 8.f });
        shaft.setOrigin({ 35.f, 4.f });
        shaft.setPosition(center);
        shaft.setFillColor(cyan);
        window.draw(shaft);

        sf::ConvexShape head(3);
        head.setPoint(0, center + sf::Vector2f(44.f, 0.f));
        head.setPoint(1, center + sf::Vector2f(18.f, -22.f));
        head.setPoint(2, center + sf::Vector2f(18.f, 22.f));
        head.setFillColor(gold);
        window.draw(head);
        return;
    }

    if (upgradeType == 5)
    {
        for (int i = 0; i < 3; ++i)
        {
            sf::RectangleShape streak({ 70.f - i * 13.f, 7.f });
            streak.setOrigin({ 35.f - i * 6.5f, 3.5f });
            streak.setPosition(center + sf::Vector2f(2.f, -23.f + i * 23.f));
            streak.setRotation(sf::degrees(-12.f));
            streak.setFillColor(i == 1 ? gold : blue);
            window.draw(streak);
        }
        return;
    }

    if (upgradeType == 6)
    {
        sf::RectangleShape vertical({ 22.f, 72.f });
        vertical.setOrigin({ 11.f, 36.f });
        vertical.setPosition(center);
        vertical.setFillColor(red);
        window.draw(vertical);

        sf::RectangleShape horizontal({ 72.f, 22.f });
        horizontal.setOrigin({ 36.f, 11.f });
        horizontal.setPosition(center);
        horizontal.setFillColor(red);
        window.draw(horizontal);
        return;
    }

    if (upgradeType == 7)
    {
        sf::CircleShape pulse(40.f);
        pulse.setOrigin({ 40.f, 40.f });
        pulse.setPosition(center);
        pulse.setFillColor(sf::Color(98, 222, 147, 70));
        pulse.setOutlineThickness(3.f);
        pulse.setOutlineColor(green);
        window.draw(pulse);

        sf::RectangleShape vertical({ 18.f, 60.f });
        vertical.setOrigin({ 9.f, 30.f });
        vertical.setPosition(center);
        vertical.setFillColor(green);
        window.draw(vertical);

        sf::RectangleShape horizontal({ 60.f, 18.f });
        horizontal.setOrigin({ 30.f, 9.f });
        horizontal.setPosition(center);
        horizontal.setFillColor(green);
        window.draw(horizontal);
        return;
    }

    sf::CircleShape ring(43.f);
    ring.setOrigin({ 43.f, 43.f });
    ring.setPosition(center);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineThickness(5.f);
    ring.setOutlineColor(blue);
    window.draw(ring);

    sf::RectangleShape cut({ 78.f, 9.f });
    cut.setOrigin({ 39.f, 4.5f });
    cut.setPosition(center);
    cut.setRotation(sf::degrees(-22.f));
    cut.setFillColor(gold);
    window.draw(cut);
}

void UIManager::DrawText(sf::RenderWindow& window, const std::string& text, unsigned int size, sf::Vector2f position, sf::Color color, bool centered)
{
    if (!_fontLoaded)
    {
        return;
    }

    sf::Text drawableText(_font, text, size);
    drawableText.setFillColor(color);

    if (centered)
    {
        const auto bounds = drawableText.getLocalBounds();
        drawableText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y });
    }

    drawableText.setPosition(position);
    window.draw(drawableText);
}

const char* UIManager::GetUpgradeName(int upgradeType)const
{
    switch (upgradeType)
    {
    case 1: return "Extra Bullet";
    case 2: return "Sharper Damage";
    case 3: return "Rapid Fire";
    case 4: return "Bullet Speed";
    case 5: return "Move Speed";
    case 6: return "Max HP";
    case 7: return "Heal";
    case 8: return "Less XP Need";
    default: return "Unknown";
    }
}

const char* UIManager::GetUpgradeDescription(int upgradeType)const
{
    switch (upgradeType)
    {
    case 1: return "+1 projectile per shot";
    case 2: return "+5 bullet damage";
    case 3: return "Shorter shot cooldown";
    case 4: return "Bullets fly faster";
    case 5: return "+20 move speed";
    case 6: return "+20 max HP and HP";
    case 7: return "Restore 35 HP";
    case 8: return "Need less XP to level";
    default: return "No effect";
    }
}
