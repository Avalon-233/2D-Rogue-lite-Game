# Integration steps

## 1. Add player UI getters

Update `LightRogue/Player.h`:

```cpp
float GetMaxHP()const;
float GetExperienceNeeded()const;
```

Update `LightRogue/Player.cpp`:

```cpp
float Player::GetMaxHP()const
{
    return _maxHP;
}

float Player::GetExperienceNeeded()const
{
    return _level * 10.f;
}
```

## 2. Add enemy UI getters

Update `LightRogue/Enemy.h`:

```cpp
float GetHP()const;
float GetMaxHP()const;
```

Also add:

```cpp
float _maxHP = 30.f;
```

Update `LightRogue/Enemy.cpp`:

```cpp
float Enemy::GetHP()const
{
    return _HP;
}

float Enemy::GetMaxHP()const
{
    return _maxHP;
}
```

## 3. Add UI files

Create:

```text
LightRogue/UI.h
LightRogue/UI.cpp
```

Use either:

- `commented/UI.h` and `commented/UI.cpp`
- `clean/UI.h` and `clean/UI.cpp`

## 4. Connect UIManager to GameWorld

Update `LightRogue/GameWorld.h`:

```cpp
#include "UI.h"
```

Add public method:

```cpp
void DrawUI(sf::RenderWindow& renderWindow);
```

Add private member:

```cpp
UIManager _uiManager;
```

Update `LightRogue/GameWorld.cpp`:

```cpp
void GameWorld::DrawUI(sf::RenderWindow& renderWindow)
{
    _uiManager.DrawPlayerHUD(renderWindow, _player.get());

    for (auto& enemy : _enemies)
    {
        _uiManager.DrawEnemyHealthBar(renderWindow, enemy.get());
    }
}
```

## 5. Draw UI after world objects

Update `LightRogue/Game.cpp`:

```cpp
_mainWindow.clear();
_gameWorld.DrawAll(_mainWindow);
_gameWorld.DrawUI(_mainWindow);
_mainWindow.display();
```

UI should be drawn after game objects so that HUD and bars are visible on top.

## 6. Add UI files to Visual Studio project

Update `LightRogue/LightRogue.vcxproj`.

Add:

```xml
<ClCompile Include="UI.cpp" />
```

Add:

```xml
<ClInclude Include="UI.h" />
```

Optionally update `LightRogue/LightRogue.vcxproj.filters` so the files appear in
the correct Visual Studio filter.
