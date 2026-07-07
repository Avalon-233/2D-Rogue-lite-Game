# LightRogue UI health and experience bar package

This directory contains a first-stage UI plan and copyable code for:

1. Player health bar
2. Enemy health bars
3. Player experience bar

The implementation is code-only and does not require art assets. It uses
`sf::RectangleShape` to draw progress bars.

## Why no assets are required

Health bars and experience bars are progress indicators. Their first version can
be drawn with two rectangles:

- background rectangle: total capacity
- fill rectangle: current value

The bar width is calculated as:

```cpp
displayWidth = totalWidth * currentValue / maxValue;
```

Art assets can be added later for borders, glow effects, or themed UI frames.

## Files

- `commented/`: teaching version with detailed comments.
- `clean/`: same code without explanatory comments.
- `integration_steps.md`: where to place each file and which project files to update.
- `ui_requirements.md`: UX/UI requirements and missing data interfaces.

## Important notes

The current `develop_y` branch already has:

- `Game`
- `GameWorld`
- `GameObject`
- `Player`
- `Enemy`
- `Projectile`
- `Pickup`

This package is intentionally uploaded as documentation/sample code instead of
directly changing the main game source. That keeps `main` safe while providing
the UI implementation plan for the team to review and merge deliberately.
