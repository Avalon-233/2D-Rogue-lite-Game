#pragma once
#include"GameObject.h"
#include"UI.h"

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

private:/*
	void UpdateEnemySpawning(float deltaTime);
	void SpawnEnemy(sf::Vector2f position);*/
	sf::Vector2f GetRandomEnemySpawnPosition()const;
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
	/*
	float _elapsedGameTime = 0.f;
	float _enemySpawnTimer = 0.f;
	float _baseEnemySpawnInterval = 2.5f;
	float _minEnemySpawnInterval = 0.45f;
	std::size_t _maxEnemyCount = 50;
	*/
	UIManager _uiManager;
	
};
