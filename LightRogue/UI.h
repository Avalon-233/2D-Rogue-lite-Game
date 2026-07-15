#pragma once

#include "SFML/Graphics.hpp"
#include"GameEvent.h"
#include <string>
#include <array>

class Player;
class Enemy;

class OverlayUI
{
public:
    virtual ~OverlayUI() = default;

    virtual void  LoadResources()=0;
    virtual void OnEnter()=0;
    virtual void UpdateHover(const sf::RenderWindow& window, sf::Vector2i position) {}
    virtual int HitTest(const sf::RenderWindow& window, sf::Vector2i position)const { return 0; }
    virtual void Draw(sf::RenderWindow& window) = 0;

    void CenterText(sf::Text& text, sf::Vector2f position)const
    {
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });
        text.setPosition(position);
    }
    static bool LoadFont(sf::Font& font)
    {
        return font.openFromFile("C:/Windows/Fonts/msyh.ttc") ||
            font.openFromFile("C:/Windows/Fonts/simhei.ttf") ||
            font.openFromFile("C:/Windows/Fonts/simsun.ttc") ||
            font.openFromFile("SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
            font.openFromFile("LightRogue/SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
            font.openFromFile("../SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
            font.openFromFile("../../SFML-3.1.0/examples/text/resources/tuffy.ttf");
    }

};

class SplashOverlayUI : public OverlayUI
{
public:
    SplashOverlayUI() = default;

    void LoadResources() override;
    void OnEnter() override;
    void UpdateHover(const sf::RenderWindow& window,sf::Vector2i position)override;
    int  HitTest(const sf::RenderWindow& window,sf::Vector2i position) const override;
    void Draw(sf::RenderWindow& window)override;
    void SetHasSaveFile(bool v) { _hasSaveFile = v; }

private:
    sf::FloatRect GetStartButtonBounds(const sf::RenderWindow& window) const;
    sf::FloatRect GetLoadButtonBounds(const sf::RenderWindow& window) const;

    sf::Clock _animationClock;
    sf::Texture _backgroundTexture;
    sf::Font _font;
    bool _backgroundReady = false;
    bool _fontReady = false;
    bool _startHovered = false;
    bool _loadHovered = false;
    bool _hasSaveFile = false;
};

class PauseOverlayUI : public OverlayUI
{
public:
    PauseOverlayUI() = default;

    void LoadResources() override;
    void OnEnter() override;
    void UpdateHover(const sf::RenderWindow& window,sf::Vector2i position)override;
    int  HitTest(const sf::RenderWindow& window,sf::Vector2i position)const override;  // 0=nathing, 1=continue, 2=save 3=exit
    void Draw(sf::RenderWindow& window)override;

private:
    sf::FloatRect GetContinueButtonBounds(const sf::RenderWindow& window) const;
    sf::FloatRect GetQuitButtonBounds(const sf::RenderWindow& window) const;
    sf::FloatRect GetSaveButtonBounds(const sf::RenderWindow& window) const;
    void DrawAnimatedButton(sf::RenderWindow& window,
        const sf::FloatRect& bounds,
        bool hovered,
        const char* labelText,
        sf::Color baseColor,
        sf::Color hoverColor,
        float phaseOffset,
        const sf::Font& font) const;

    sf::Font  _font;
    sf::Clock _animationClock;
    bool _fontReady = false;
    bool _continueHovered = false;
    bool _quitHovered = false;
    bool _saveHovered = false;
};

class UpgradeOverlayUI : public OverlayUI
{
public:
    UpgradeOverlayUI() = default;

    void LoadResources() override;       // 只需字体
    void OnEnter() override;
    void OnEnter(const std::array<int, 3>& options);    // 重载：带选项数据

    // UpdateHover：卡片高亮（后续可加）
    int  HitTest(const sf::RenderWindow& window,sf::Vector2i position) const override;  // 0=未命中, 1/2/3=卡片

    void Draw(sf::RenderWindow& window)override;

private:
    void DrawUpgradeIcon(sf::RenderWindow& window,
        sf::Vector2f center, int upgradeType) const;
    static const char* GetUpgradeName(int upgradeType);
    static const char* GetUpgradeDescription(int upgradeType);
    // 布局常量（与 DrawUpgradeChoices 保持一致）
    static constexpr sf::Vector2f CardSize = { 250.f, 365.f };
    static constexpr float        CardGap = 38.f;
    static constexpr float        CardY = 184.f;

    sf::FloatRect GetCardBounds(int cardIndex, float windowWidth) const;
    void DrawOneCard(sf::RenderWindow& window,
        sf::Vector2f position, int shortcut,
        int upgradeType, const sf::Font& font) const;

    sf::Font                _font;
    bool                    _fontReady = false;
    std::array<int, 3>      _cachedOptions = { 1, 2, 3 };  // OnEnter 时快照
};

class GameOverOverlayUI : public OverlayUI
{
public:
    GameOverOverlayUI() = default;

    void LoadResources() override;
    void OnEnter() override;
    void OnEnter(float gameTime, int score, int level);    // 带数据重载

    void Draw(sf::RenderWindow& window) override;

private:
    sf::Font _font;
    bool _fontReady = false;
    float _gameTime = 0.f;
    int   _score = 0;
    int   _level = 1;
};

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

    void OnSplashEnter();
    void OnPauseEnter();
    void OnUpgradeEnter(const std::array<int, 3>& options);
    void OnGameOverEnter(float time, int score, int level);
    void LoadOverlayResources();
    void UpdateOverlayHover(const sf::RenderWindow& window,sf::Vector2i position);
    int  HitTestOverlay(const sf::RenderWindow& window,sf::Vector2i position) const;
    void DrawOverlay(sf::RenderWindow& window);
    void DrawFPS(sf::RenderWindow& window, int fps);
    void ActivateOverlay(GameState state);
    void SetSplashHasSaveFile(bool v) { _splashUI.SetHasSaveFile(v); }

    void DrawPlayerHUD(sf::RenderWindow& window, const Player* player);

private:
    
    void DrawText(sf::RenderWindow& window, const std::string& text,
        unsigned int size, sf::Vector2f position,
        sf::Color color, bool centered, const sf::Font& font);

    SplashOverlayUI    _splashUI;
    PauseOverlayUI     _pauseUI;
    UpgradeOverlayUI   _upgradeUI;
    GameOverOverlayUI  _gameOverUI;
    OverlayUI* _activeOverlay = nullptr;

    StatusBar _playerHealthBar;
    StatusBar _playerExperienceBar;
    sf::Font _hudFont;
    bool     _hudFontReady = false;

};
