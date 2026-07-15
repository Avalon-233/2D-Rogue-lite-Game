# UI requirements: health bars and experience bar

## Player health bar

Purpose:

Let the player always know their survival state.

Display:

- screen-space HUD
- fixed in the top-left corner

Data required:

- current HP: `Player::GetHP()`
- max HP: `Player::GetMaxHP()`

Current code status:

- `Player::GetHP()` already exists.
- `Player::_maxHP` already exists.
- `Player::GetMaxHP()` is missing and should be added.

## Player experience bar

Purpose:

Let the player know their growth progress toward the next level.

Display:

- screen-space HUD
- below the player health bar

Data required:

- current experience: `Player::GetExperience()`
- next-level requirement: `Player::GetExperienceNeeded()`
- current level: `Player::GetLevel()`

Current code status:

- `Player::GetExperience()` already exists.
- `Player::GetLevel()` already exists.
- level-up rule is currently `level * 10.f`.
- `Player::GetExperienceNeeded()` is missing and should be added.

## Enemy health bar

Purpose:

Let the player understand enemy durability and attack feedback.

Display:

- world-space UI
- above each enemy

Data required:

- enemy current HP: `Enemy::GetHP()`
- enemy max HP: `Enemy::GetMaxHP()`
- enemy world position: `Enemy::GetPosition()`

Current code status:

- `Enemy::_HP` exists.
- `Enemy::_maxHP` should be added.
- `Enemy::GetHP()` and `Enemy::GetMaxHP()` are missing and should be added.

## Recommended UI classes

`StatusBar`

- generic rectangle-based progress bar
- reusable for health bars and experience bars

`UIManager`

- draws player HUD
- draws enemy health bars

## First-stage scope

Included:

- player HP bar
- player EXP bar
- enemy HP bars

Not included yet:

- text labels
- animated damage numbers
- UI art frames
- low-health warning
- level-up popup
- pause menu
- game-over screen
