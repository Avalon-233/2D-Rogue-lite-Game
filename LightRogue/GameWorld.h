#pragma once
#include"GameObject.h"
#include"UI.h"

#include <string>

class Player;
class Enemy;
class Projectile;
class Pickup;

class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	
	void Add(std::unique_ptr<Player> player);
	void Add(std::unique_ptr<Enemy> enemy);
	void Add(std::unique_ptr<Projectile> projectile);
	void Add(std::unique_ptr<Pickup> pickup);

	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll(float deltaTime);

	void Collision();

	void CleanUp();

	void DrawUI(sf::RenderWindow& renderWindow);
	void DrawUpgradeUI(sf::RenderWindow& renderWindow);
	bool IsPlayerDead()const;
	bool IsTimeUp()const;
	bool HasPendingUpgrade()const;
	void ApplyUpgradeOption(int option);
	float GetGameTime()const;
	std::string GetUpgradePrompt()const;

private:
	
	sf::Vector2f GetRandomSpawnPosition()const;
	void UpdateSpawning(float deltaTime);
	
	std::unique_ptr<Player> _player;
	std::vector<std::unique_ptr<Enemy>> _enemies;
	std::vector<std::unique_ptr<Projectile>> _projectiles;
	std::vector<std::unique_ptr<Pickup>> _pickups;
	sf::Texture _pickupTexture;
	sf::Texture _enemyTexture;

	float _gameTime = 0.f;
	float _spawnTimer = 0.f;
	const int MAX_ENEMIES_COUNT = 50;
	float _minInterval = 0.45f;
	float _baseInterval = 2.5f;
	float _decayRate = 0.03f;
	float _gameTimeLimit = 360.f;

	UIManager _uiManager;
	
};
