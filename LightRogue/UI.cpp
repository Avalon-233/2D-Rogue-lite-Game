#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"
#include"ResourceLoader.h"

void UIManager::ActivateOverlay(GameState state)
{
    switch (state)
    {
    case ShowingSplash: _activeOverlay = &_splashUI; break;
    case Paused:        _activeOverlay = &_pauseUI;  break;
    case Upgrading:     _activeOverlay = &_upgradeUI; break;
    case GameOver:      _activeOverlay = &_gameOverUI; break;
    default:            _activeOverlay = nullptr;     break;
    }
}

void UIManager::UpdateOverlayHover(const sf::RenderWindow& window,sf::Vector2i position)
{
    if (_activeOverlay)
        _activeOverlay->UpdateHover(window, position);
}

int UIManager::HitTestOverlay(const sf::RenderWindow& window,sf::Vector2i position) const
{
    return _activeOverlay ? _activeOverlay->HitTest(window, position) : 0;
}

void UIManager::DrawOverlay(sf::RenderWindow& window)
{
    if (_activeOverlay)
        _activeOverlay->Draw(window);
}

void UIManager::DrawFPS(sf::RenderWindow& window, int fps)
{
    if (!_hudFontReady) return;

    const sf::Vector2f pos(1420.f, 10.f);
    const sf::Color color(180, 220, 180);

    sf::Text text(_hudFont, std::to_string(fps) + " FPS", 18);
    text.setFillColor(color);
    text.setPosition(pos);

    const sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x, 0.f });

    window.draw(text);
}


void UIManager::LoadOverlayResources()
{
    _splashUI.LoadResources();
    _pauseUI.LoadResources();
    _upgradeUI.LoadResources();
    _gameOverUI.LoadResources();
    _hudFontReady = OverlayUI::LoadFont(_hudFont);
}

void UIManager::OnSplashEnter()
{
    _splashUI.OnEnter();
}

void UIManager::OnPauseEnter()
{
    _pauseUI.OnEnter();
}

void UIManager::OnUpgradeEnter(const std::array<int, 3>& options)
{
    _upgradeUI.OnEnter(options);
}

void UIManager::OnGameOverEnter(float time, int score, int level)
{
    _gameOverUI.OnEnter(time, score, level);
}
void SplashOverlayUI::LoadResources()
{
    _backgroundReady = LoadTextureFromResource(_backgroundTexture, "SplashScreen.png");

    if (_backgroundReady)
        _backgroundTexture.setSmooth(true);

    _fontReady = OverlayUI::LoadFont(_font);
}

void SplashOverlayUI::OnEnter()
{
    _animationClock.restart();
    _startHovered = false;
    _loadHovered = false;
}

void SplashOverlayUI::UpdateHover(const sf::RenderWindow& window, sf::Vector2i position)
{
    const sf::Vector2f mousePoint(
        static_cast<float>(position.x),
        static_cast<float>(position.y)
    );
    _startHovered = GetStartButtonBounds(window).contains(mousePoint);
    _loadHovered = _hasSaveFile && GetLoadButtonBounds(window).contains(mousePoint);
}

int SplashOverlayUI::HitTest(const sf::RenderWindow& window, sf::Vector2i position) const
{
    const sf::Vector2f mousePoint(
        static_cast<float>(position.x),
        static_cast<float>(position.y)
    );
    if (_hasSaveFile && GetLoadButtonBounds(window).contains(mousePoint)) return 2;
    return GetStartButtonBounds(window).contains(mousePoint) ? 1 : 0;
}

void SplashOverlayUI::Draw(sf::RenderWindow& window)
{
    const sf::Vector2f windowSize(sf::Vector2f(1440.f, 810.f));
    const sf::Vector2f center(windowSize.x / 2.f, windowSize.y / 2.f);

    if (_backgroundReady)
    {
        sf::Sprite background(_backgroundTexture);
        const sf::Vector2u textureSize = _backgroundTexture.getSize();
        const float scale = std::max(
            windowSize.x / static_cast<float>(textureSize.x),
            windowSize.y / static_cast<float>(textureSize.y)
        );
        background.setScale({ scale, scale });
        background.setPosition({
            (windowSize.x - static_cast<float>(textureSize.x) * scale) / 2.f,
            (windowSize.y - static_cast<float>(textureSize.y) * scale) / 2.f
            });
        window.draw(background);
    }
    else
    {
        sf::RectangleShape bg(windowSize);
        bg.setFillColor(sf::Color(17, 19, 30));
        window.draw(bg);
    }

    sf::RectangleShape shade(windowSize);
    shade.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(shade);

    // 刷新悬停状态
    int hit = HitTest(window, sf::Mouse::getPosition(window));
    _startHovered = (hit == 1);
    _loadHovered = _hasSaveFile && (hit == 2);
    const float time = _animationClock.getElapsedTime().asSeconds();
    const sf::FloatRect buttonBounds = GetStartButtonBounds(window);
    const sf::Vector2f buttonCenter(
        buttonBounds.position.x + buttonBounds.size.x / 2.f,
        buttonBounds.position.y + buttonBounds.size.y / 2.f
    );
    const float pulse = _startHovered
        ? std::sin(time * 8.f) * 4.f : std::sin(time * 2.f) * 2.f;
    const sf::Vector2f buttonSize(
        buttonBounds.size.x + (_startHovered ? 28.f : 0.f) + pulse,
        buttonBounds.size.y + (_startHovered ? 12.f : 0.f) + pulse
    );

    sf::RectangleShape glow(buttonSize + sf::Vector2f(24.f, 18.f));
    glow.setOrigin(glow.getSize() / 2.f);
    glow.setPosition(buttonCenter);
    glow.setFillColor(_startHovered
        ? sf::Color(255, 205, 90, 70) : sf::Color(80, 140, 255, 35));
    window.draw(glow);

    sf::RectangleShape button(buttonSize);
    button.setOrigin(buttonSize / 2.f);
    button.setPosition(buttonCenter);
    button.setFillColor(_startHovered
        ? sf::Color(255, 205, 90, 230) : sf::Color(26, 31, 48, 230));
    button.setOutlineThickness(3.f);
    button.setOutlineColor(_startHovered
        ? sf::Color(255, 245, 190) : sf::Color(115, 170, 255));
    window.draw(button);

    if (!_fontReady) return;

    sf::Text title(_font, "LightRogue", 74);
    title.setFillColor(sf::Color(245, 248, 255));
    CenterText(title, { center.x, 150.f });
    window.draw(title);

    sf::Text subtitle(_font, "ESC exits. Click the button starts.", 24);
    subtitle.setFillColor(sf::Color(200, 215, 235));
    CenterText(subtitle, { center.x, 220.f });
    window.draw(subtitle);

    sf::Text startText(_font, "NEW GAME", 
        _startHovered ? 50u : 46u);
    startText.setFillColor(_startHovered
        ? sf::Color(24, 28, 42) : sf::Color(245, 248, 255));
    CenterText(startText, buttonCenter + sf::Vector2f(0.f, _startHovered ? -3.f : 0.f));
    window.draw(startText);

    if (_hasSaveFile)
    {
        const sf::FloatRect loadBounds = GetLoadButtonBounds(window);
        const sf::Vector2f loadCenter(
            loadBounds.position.x + loadBounds.size.x / 2.f,
            loadBounds.position.y + loadBounds.size.y / 2.f);
        const float loadPulse = _loadHovered
            ? std::sin(time * 8.f + 0.5f) * 4.f : std::sin(time * 2.f + 0.5f) * 2.f;
        const sf::Vector2f loadSize(
            loadBounds.size.x + (_loadHovered ? 28.f : 0.f) + loadPulse,
            loadBounds.size.y + (_loadHovered ? 12.f : 0.f) + loadPulse);

        sf::RectangleShape lGlow(loadSize + sf::Vector2f(24.f, 18.f));
        lGlow.setOrigin(lGlow.getSize() / 2.f);
        lGlow.setPosition(loadCenter);
        lGlow.setFillColor(_loadHovered
            ? sf::Color(80, 180, 255, 70) : sf::Color(80, 140, 255, 35));
        window.draw(lGlow);

        sf::RectangleShape lBtn(loadSize);
        lBtn.setOrigin(loadSize / 2.f);
        lBtn.setPosition(loadCenter);
        lBtn.setFillColor(_loadHovered
            ? sf::Color(80, 180, 255, 230) : sf::Color(26, 31, 48, 230));
        lBtn.setOutlineThickness(3.f);
        lBtn.setOutlineColor(_loadHovered
            ? sf::Color(180, 220, 255) : sf::Color(115, 170, 255));
        window.draw(lBtn);

        sf::Text loadText(_font, "LOAD GAME", _loadHovered ? 42u : 38u);
        loadText.setFillColor(_loadHovered
            ? sf::Color(24, 28, 42) : sf::Color(200, 220, 255));
        CenterText(loadText, loadCenter + sf::Vector2f(0.f, _loadHovered ? -3.f : 0.f));
        window.draw(loadText);
    }

}

sf::FloatRect SplashOverlayUI::GetStartButtonBounds(
    const sf::RenderWindow& window) const
{
    const sf::Vector2u pixelSize = sf::Vector2u(1440.f, 810.f);
    const float centerX = static_cast<float>(pixelSize.x) / 2.f;
    const float centerY = static_cast<float>(pixelSize.y) * 0.66f;
    return sf::FloatRect({ centerX - 170.f, centerY - 44.f }, { 340.f, 88.f });
}

sf::FloatRect SplashOverlayUI::GetLoadButtonBounds(
    const sf::RenderWindow& window) const
{
    const sf::Vector2u pixelSize = sf::Vector2u(1440.f, 810.f);
    const float centerX = static_cast<float>(pixelSize.x) / 2.f;
    const float centerY = static_cast<float>(pixelSize.y) * 0.66f + 100.f;
    return sf::FloatRect({ centerX - 170.f, centerY - 44.f }, { 340.f, 88.f });
}


void PauseOverlayUI::LoadResources()
{
    _fontReady = OverlayUI::LoadFont(_font);
}

void PauseOverlayUI::OnEnter()
{
    _animationClock.restart();
    _continueHovered = false;
    _saveHovered = false;
    _quitHovered = false;
}

void PauseOverlayUI::UpdateHover(const sf::RenderWindow& window,
    sf::Vector2i position)
{
    const sf::Vector2f mousePoint(
        static_cast<float>(position.x),
        static_cast<float>(position.y)
    );
    _continueHovered = GetContinueButtonBounds(window).contains(mousePoint);
    _quitHovered = GetQuitButtonBounds(window).contains(mousePoint);
    _saveHovered = GetSaveButtonBounds(window).contains(mousePoint);
}

int PauseOverlayUI::HitTest(const sf::RenderWindow& window,
    sf::Vector2i position) const
{
    const sf::Vector2f mousePoint(
        static_cast<float>(position.x),
        static_cast<float>(position.y)
    );
    if (GetContinueButtonBounds(window).contains(mousePoint)) return 1;
    if (GetQuitButtonBounds(window).contains(mousePoint))     return 3;
    if (GetSaveButtonBounds(window).contains(mousePoint))     return 2;
    return 0;
}

void PauseOverlayUI::Draw(sf::RenderWindow& window)
{
    const sf::Vector2f windowSize(sf::Vector2u(1440.f, 810.f));
    const sf::Vector2f center(windowSize.x / 2.f, windowSize.y / 2.f);

    // 刷新悬停
    UpdateHover(window, sf::Mouse::getPosition(window));
    const float time = _animationClock.getElapsedTime().asSeconds();

    // 遮罩
    sf::RectangleShape overlay(windowSize);
    overlay.setFillColor(sf::Color(4, 7, 22, 190));
    window.draw(overlay);

    if (!_fontReady) return;

    sf::Text title(_font, "PAUSED", 64);
    title.setFillColor(sf::Color(238, 212, 132));
    CenterText(title, { center.x, center.y - 160.f });
    window.draw(title);

    sf::Text subtitle(_font, "Press ESC or click a button", 22);
    subtitle.setFillColor(sf::Color(151, 169, 188));
    CenterText(subtitle, { center.x, center.y - 100.f });
    window.draw(subtitle);

    // Continue button
    DrawAnimatedButton(window, GetContinueButtonBounds(window),
        _continueHovered, "CONTINUE",
        sf::Color(255, 205, 90), sf::Color(255, 220, 120), 0.f, _font);

    // Quit button
    DrawAnimatedButton(window, GetQuitButtonBounds(window),
        _quitHovered, "QUIT GAME",
        sf::Color(180, 80, 60), sf::Color(220, 100, 80), 1.f, _font);

    DrawAnimatedButton(window, GetSaveButtonBounds(window),
        _saveHovered, "SAVE",
        sf::Color(80, 160, 220), sf::Color(120, 200, 255), 0.5f, _font);

    sf::Text hint(_font, "Move the mouse over a button, then click.", 18);
    hint.setFillColor(sf::Color(140, 150, 170));
    CenterText(hint, { center.x, center.y + 260.f });
    window.draw(hint);
}

sf::FloatRect PauseOverlayUI::GetContinueButtonBounds(
    const sf::RenderWindow& window) const
{
    const float cx = static_cast<float>(sf::Vector2u(1440.f, 810.f).x) / 2.f;
    const float cy = static_cast<float>(sf::Vector2u(1440.f, 810.f).y) * 0.48f;
    return sf::FloatRect({ cx - 200.f, cy - 40.f }, { 400.f, 80.f });
}

sf::FloatRect PauseOverlayUI::GetQuitButtonBounds(
    const sf::RenderWindow& window) const
{
    const float cx = static_cast<float>(sf::Vector2u(1440.f, 810.f).x) / 2.f;
    const float cy = static_cast<float>(sf::Vector2u(1440.f, 810.f).y) * 0.48f + 180.f;
    return sf::FloatRect({ cx - 200.f, cy - 40.f }, { 400.f, 80.f });
}

sf::FloatRect PauseOverlayUI::GetSaveButtonBounds(
    const sf::RenderWindow& window) const
{
    const float cx = static_cast<float>(sf::Vector2u(1440.f, 810.f).x) / 2.f;
    const float cy = static_cast<float>(sf::Vector2u(1440.f, 810.f).y) * 0.48f + 90.f;
    return sf::FloatRect({ cx - 200.f, cy - 40.f }, { 400.f, 80.f });
}


void PauseOverlayUI::DrawAnimatedButton(sf::RenderWindow& window,
    const sf::FloatRect& bounds, bool hovered,
    const char* labelText, sf::Color baseColor, sf::Color hoverColor,
    float phaseOffset, const sf::Font& font) const
{
    const float time = _animationClock.getElapsedTime().asSeconds();
    const sf::Vector2f btnCenter(
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    );
    const float pulse = hovered
        ? std::sin(time * 8.f + phaseOffset) * 4.f
        : std::sin(time * 2.f + phaseOffset) * 2.f;
    const sf::Vector2f btnSize(
        bounds.size.x + (hovered ? 28.f : 0.f) + pulse,
        bounds.size.y + (hovered ? 12.f : 0.f) + pulse
    );

    sf::RectangleShape glow(btnSize + sf::Vector2f(24.f, 18.f));
    glow.setOrigin(glow.getSize() / 2.f);
    glow.setPosition(btnCenter);
    glow.setFillColor(hovered
        ? sf::Color(hoverColor.r, hoverColor.g, hoverColor.b, 70)
        : sf::Color(baseColor.r, baseColor.g, baseColor.b, 30));
    window.draw(glow);

    sf::RectangleShape button(btnSize);
    button.setOrigin(btnSize / 2.f);
    button.setPosition(btnCenter);
    button.setFillColor(hovered
        ? sf::Color(hoverColor.r, hoverColor.g, hoverColor.b, 230)
        : sf::Color(26, 31, 48, 230));
    button.setOutlineThickness(3.f);
    button.setOutlineColor(hovered
        ? sf::Color(std::min(255, hoverColor.r + 50),
            std::min(255, hoverColor.g + 50),
            std::min(255, hoverColor.b + 50))
        : sf::Color(std::min(255, baseColor.r + 30),
            std::min(255, baseColor.g + 30),
            std::min(255, baseColor.b + 30)));
    window.draw(button);

    sf::Text label(font, labelText, hovered ? 42u : 38u);
    label.setFillColor(hovered
        ? sf::Color(24, 28, 42) : sf::Color(245, 248, 255));
    // 手动居中（不用 CenterText，因为它是 const 且需要非 const）
    {
        const sf::FloatRect b = label.getLocalBounds();
        label.setOrigin({ b.position.x + b.size.x / 2.f,
                          b.position.y + b.size.y / 2.f });
    }
    label.setPosition(btnCenter + sf::Vector2f(0.f, hovered ? -3.f : 0.f));
    window.draw(label);
}

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
    /*
    _fontLoaded =
        _font.openFromFile("SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("LightRogue/SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("../SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("../../SFML-3.1.0/examples/text/resources/tuffy.ttf") ||
        _font.openFromFile("C:/Windows/Fonts/segoeui.ttf");
        */
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
    /*
    _enemyHealthBar.SetSize(42.f, 6.f);
    _enemyHealthBar.SetColors(
        sf::Color(30, 30, 30),
        sf::Color(230, 60, 60)
    );*/
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

void UpgradeOverlayUI::LoadResources()
{
    _fontReady = OverlayUI::LoadFont(_font);
}

void UpgradeOverlayUI::OnEnter() {}

void UpgradeOverlayUI::OnEnter(const std::array<int, 3>& options)
{
    _cachedOptions = options;
}

int UpgradeOverlayUI::HitTest(const sf::RenderWindow& window,
    sf::Vector2i position) const
{
    const float windowWidth = static_cast<float>(sf::Vector2u(1440.f, 810.f).x);
    for (int i = 0; i < 3; ++i)
    {
        if (GetCardBounds(i, windowWidth).contains(
            { static_cast<float>(position.x),
              static_cast<float>(position.y) }))
            return i + 1;
    }
    return 0;
}

void UpgradeOverlayUI::Draw(sf::RenderWindow& window)
{
    if (!_fontReady) return;

    const sf::Vector2f windowSize(sf::Vector2u(1440.f, 810.f));
    const float startX = windowSize.x / 2.f
        - (CardSize.x * 3.f + CardGap * 2.f) / 2.f;

    // 遮罩
    sf::RectangleShape overlay(windowSize);
    overlay.setFillColor(sf::Color(4, 7, 18, 218));
    window.draw(overlay);

    // 顶部装饰线
    sf::RectangleShape topRule({ 660.f, 2.f });
    topRule.setPosition({ windowSize.x / 2.f - 330.f, 104.f });
    topRule.setFillColor(sf::Color(68, 214, 221, 150));
    window.draw(topRule);

    // 标题
    sf::Text title(_font, "CHOOSE YOUR LIGHT CODE", 34);
    title.setFillColor(sf::Color(238, 212, 132));
    CenterText(title, { windowSize.x / 2.f, 58.f });
    window.draw(title);

    sf::Text subtitle(_font, "Press 1/2/3 or click a card", 18);
    subtitle.setFillColor(sf::Color(178, 226, 229));
    CenterText(subtitle, { windowSize.x / 2.f, 112.f });
    window.draw(subtitle);

    // 三张卡片
    for (int i = 0; i < 3; ++i)
    {
        DrawOneCard(window,
            { startX + i * (CardSize.x + CardGap), CardY },
            i + 1, _cachedOptions[i], _font);
    }

    // 底部线
    sf::RectangleShape bottomRule({ 520.f, 2.f });
    bottomRule.setPosition({ windowSize.x / 2.f - 260.f, 604.f });
    bottomRule.setFillColor(sf::Color(198, 164, 72, 135));
    window.draw(bottomRule);

    sf::Text hint(_font, "The battle is paused while this panel is open", 16);
    hint.setFillColor(sf::Color(151, 169, 188));
    CenterText(hint, { windowSize.x / 2.f, 632.f });
    window.draw(hint);
}

sf::FloatRect UpgradeOverlayUI::GetCardBounds(int cardIndex,
    float windowWidth) const
{
    const float totalWidth = CardSize.x * 3.f + CardGap * 2.f;
    const float startX = windowWidth / 2.f - totalWidth / 2.f;
    const float left = startX + cardIndex * (CardSize.x + CardGap);
    return sf::FloatRect({ left, CardY }, { CardSize.x, CardSize.y });
}

void UpgradeOverlayUI::DrawOneCard(sf::RenderWindow& window,
    sf::Vector2f position, int shortcut, int upgradeType,
    const sf::Font& font) const
{
    const sf::Color cardFill(10, 17, 34, 238);
    const sf::Color cyan(74, 218, 226);
    const sf::Color gold(218, 178, 79);
    const sf::Color muted(142, 161, 184);

    sf::RectangleShape shadow(CardSize);
    shadow.setPosition(position + sf::Vector2f(8.f, 10.f));
    shadow.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(shadow);

    sf::RectangleShape outer(CardSize);
    outer.setPosition(position);
    outer.setFillColor(cardFill);
    outer.setOutlineThickness(3.f);
    outer.setOutlineColor(gold);
    window.draw(outer);

    sf::RectangleShape inner(CardSize - sf::Vector2f(22.f, 22.f));
    inner.setPosition(position + sf::Vector2f(11.f, 11.f));
    inner.setFillColor(sf::Color::Transparent);
    inner.setOutlineThickness(1.f);
    inner.setOutlineColor(sf::Color(74, 218, 226, 160));
    window.draw(inner);

    sf::RectangleShape topGlow({ CardSize.x - 36.f, 4.f });
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
    rightCorner.setPoint(0, position + sf::Vector2f(CardSize.x, CardSize.y));
    rightCorner.setPoint(1, position + sf::Vector2f(CardSize.x - 38.f, CardSize.y));
    rightCorner.setPoint(2, position + sf::Vector2f(CardSize.x, CardSize.y - 38.f));
    rightCorner.setFillColor(sf::Color(218, 178, 79, 150));
    window.draw(rightCorner);

    sf::CircleShape badge(36.f, 6);
    badge.setOrigin({ 36.f, 36.f });
    badge.setPosition(position + sf::Vector2f(CardSize.x / 2.f, 0.f));
    badge.setRotation(sf::degrees(30.f));
    badge.setFillColor(sf::Color(13, 29, 49));
    badge.setOutlineThickness(3.f);
    badge.setOutlineColor(gold);
    window.draw(badge);

    // 快捷键数字
    sf::Text numText(font, std::to_string(shortcut), 30);
    numText.setFillColor(sf::Color(244, 232, 174));
    {
        const sf::FloatRect b = numText.getLocalBounds();
        numText.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y });
    }
    numText.setPosition(position + sf::Vector2f(CardSize.x / 2.f, -19.f));
    window.draw(numText);

    sf::CircleShape iconPlate(68.f, 6);
    iconPlate.setOrigin({ 68.f, 68.f });
    iconPlate.setPosition(position + sf::Vector2f(CardSize.x / 2.f, 126.f));
    iconPlate.setRotation(sf::degrees(30.f));
    iconPlate.setFillColor(sf::Color(7, 35, 52));
    iconPlate.setOutlineThickness(2.f);
    iconPlate.setOutlineColor(sf::Color(74, 218, 226, 190));
    window.draw(iconPlate);

    DrawUpgradeIcon(window, position + sf::Vector2f(CardSize.x / 2.f, 126.f),
        upgradeType);

    // 升级名称
    {
        sf::Text nameText(font, GetUpgradeName(upgradeType), 23);
        nameText.setFillColor(sf::Color(241, 225, 164));
        const sf::FloatRect b = nameText.getLocalBounds();
        nameText.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y });
        nameText.setPosition(position + sf::Vector2f(CardSize.x / 2.f, 218.f));
        window.draw(nameText);
    }

    // 升级描述
    {
        sf::Text descText(font, GetUpgradeDescription(upgradeType), 16);
        descText.setFillColor(sf::Color(190, 215, 220));
        const sf::FloatRect b = descText.getLocalBounds();
        descText.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y });
        descText.setPosition(position + sf::Vector2f(CardSize.x / 2.f, 262.f));
        window.draw(descText);
    }

    sf::RectangleShape chooseBar({ CardSize.x - 68.f, 32.f });
    chooseBar.setPosition(position + sf::Vector2f(34.f, CardSize.y - 62.f));
    chooseBar.setFillColor(sf::Color(21, 43, 62, 230));
    chooseBar.setOutlineThickness(1.f);
    chooseBar.setOutlineColor(sf::Color(218, 178, 79, 170));
    window.draw(chooseBar);

    sf::Text pressText(font, "Press " + std::to_string(shortcut), 16);
    pressText.setFillColor(muted);
    {
        const sf::FloatRect b = pressText.getLocalBounds();
        pressText.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y });
    }
    pressText.setPosition(position + sf::Vector2f(CardSize.x / 2.f, CardSize.y - 55.f));
    window.draw(pressText);
}

void UpgradeOverlayUI::DrawUpgradeIcon(sf::RenderWindow& window,
    sf::Vector2f center, int upgradeType) const
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

void UIManager::DrawText(sf::RenderWindow& window,
    const std::string& text, unsigned int size,
    sf::Vector2f position, sf::Color color,
    bool centered, const sf::Font& font)
{
    sf::Text drawableText(font, text, size);
    drawableText.setFillColor(color);

    if (centered)
    {
        const auto bounds = drawableText.getLocalBounds();
        drawableText.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y });
    }

    drawableText.setPosition(position);
    window.draw(drawableText);
}

const char* UpgradeOverlayUI::GetUpgradeName(int upgradeType)
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

const char* UpgradeOverlayUI::GetUpgradeDescription(int upgradeType)
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

void GameOverOverlayUI::LoadResources()
{
    _fontReady = OverlayUI::LoadFont(_font);
}

void GameOverOverlayUI::OnEnter() {}

void GameOverOverlayUI::OnEnter(float gameTime, int score, int level)
{
    _gameTime = gameTime;
    _score = score;
    _level = level;
}

void GameOverOverlayUI::Draw(sf::RenderWindow& window)
{
    if (!_fontReady) return;

    const sf::Vector2f windowSize(sf::Vector2u(1440.f, 810.f));

    sf::RectangleShape overlay(windowSize);
    overlay.setFillColor(sf::Color(20, 4, 4, 220));
    window.draw(overlay);

    sf::Text title(_font, "GAME OVER", 80);
    title.setFillColor(sf::Color(236, 89, 92));
    CenterText(title, { windowSize.x / 2.f, windowSize.y / 2.f - 140.f });
    window.draw(title);

    sf::RectangleShape divider({ 400.f, 2.f });
    divider.setPosition({ windowSize.x / 2.f - 200.f, windowSize.y / 2.f - 70.f });
    divider.setFillColor(sf::Color(236, 89, 92, 120));
    window.draw(divider);

    char buf[256];
    snprintf(buf, sizeof(buf),
        "Level %d  |  Survived %.0f seconds  |  %d Total Score",
        _level, _gameTime, _score);
    sf::Text stats(_font, buf, 28);
    stats.setFillColor(sf::Color(178, 226, 229));
    CenterText(stats, { windowSize.x / 2.f, windowSize.y / 2.f - 40.f });
    window.draw(stats);

    sf::Text hint(_font, "( Press any key to quit )", 20);
    hint.setFillColor(sf::Color(151, 169, 188));
    CenterText(hint, { windowSize.x / 2.f, windowSize.y / 2.f + 80.f });
    window.draw(hint);
}
