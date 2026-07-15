#pragma once
#include"GameObject.h"
#include"UI.h"
#include"Enemy.h"
#include "SoundManager.h" 
#include "SaveData.h" 

#include <string>

class Player;
class Projectile;
class Pickup;

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void SaveGame()const;
	bool LoadGame();
	
	void Add(std::unique_ptr<Player> player);
	void Add(std::unique_ptr<Enemy> enemy);
	void Add(std::unique_ptr<Projectile> projectile);
	void Add(std::unique_ptr<Pickup> pickup);

	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll(float deltaTime);

	void Collision();

	void CleanUp();

	void DrawUI(sf::RenderWindow& renderWindow);

	bool IsPlayerDead()const;
	bool IsTimeUp()const;
	bool HasPendingUpgrade()const;
	void ApplyUpgradeOption(int option);
	float GetGameTime()const;
	int GetScore()const;
	int GetPlayerLevel()const;
	const std::array<int, 3>& GetPlayerUpgradeOptions() const;
	const sf::Texture& GetEnemyProjectileTexture() const;
	std::string GetUpgradePrompt()const;
	UIManager& GetUIManager();
	SoundManager& GetSoundManager();

private:
	struct TimedEffect
	{
		sf::Sprite sprite;
		float remaining;

		TimedEffect(const sf::Texture& texture, sf::Vector2f position)
			: sprite(texture), remaining(0.2f)
		{
			sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
			sprite.setPosition(position);
			sprite.setScale({ 2.f, 2.f });
		}
	};
	
	sf::Vector2f GetRandomSpawnPosition()const;
	void UpdateSpawning(float deltaTime);
	const sf::Texture& GetEnemyTexture(EnemyType type)const;
	
	std::unique_ptr<Player> _player;
	std::vector<std::unique_ptr<Enemy>> _enemies;
	std::vector<std::unique_ptr<Projectile>> _projectiles;
	std::vector<std::unique_ptr<Pickup>> _pickups;
	std::vector<TimedEffect> _effects;

	sf::Texture _backgroundTexture;
	sf::Texture _pickupExperienceTexture;
	sf::Texture _pickupHealthTexture;
	sf::Texture _enemyBasicTexture;
	sf::Texture _enemyRangedTexture;
	sf::Texture _enemyBomberTexture;
	sf::Texture _enemyGiantTexture;
	sf::Texture _enemyProjectileTexture;
	sf::Texture _explosionTexture;

	float _gameTime = 0.f;
	float _spawnTimer = 0.f;
	const int MAX_ENEMIES_COUNT = 50;
	float _minInterval = 0.35f;
	float _baseInterval = 2.5f;
	float _decayRate = 0.025f;
	float _gameTimeLimit = 360.f;

	int _scoreCounter = 0;

	UIManager _uiManager;
	SoundManager  _soundManager;
	
};
