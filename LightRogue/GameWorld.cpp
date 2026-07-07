#include"stdafx.h"
#include"GameWorld.h"
#include"GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"

namespace
{
	const sf::Vector2f GameAreaSize(1024.f, 768.f);
	constexpr float EnemySpawnMargin = 40.f;

	std::mt19937& GetRandomEngine()
	{
		static std::random_device seedGenerator;
		static std::mt19937 engine(seedGenerator());
		return engine;
	}

	float RandomFloat(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(GetRandomEngine());
	}

	int RandomInt(int min, int max)
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(GetRandomEngine());
	}
}

GameWorld::GameWorld()
{
	sf::Image pickupImage({ 16,16 }, sf::Color::Yellow);
	assert(_pickupTexture.loadFromImage(pickupImage));
	sf::Image enemyImage({ 32,32 }, sf::Color::Red);
	assert(_enemyTexture.loadFromImage(enemyImage));
}

GameWorld::~GameWorld()
{
}

void GameWorld::Add(std::unique_ptr<Player> player)
{
	_player = std::move(player);
}

void GameWorld::Add(std::unique_ptr<Enemy> enemy)
{
	_enemies.push_back(std::move(enemy));
}

void GameWorld::Add(std::unique_ptr<Projectile> projectile)
{
	_projectiles.push_back(std::move(projectile));
}

void GameWorld::Add(std::unique_ptr<Pickup> pickup)
{
	_pickups.push_back(std::move(pickup));
}

void GameWorld::DrawAll(sf::RenderWindow& renderWindow)
{
	if(_player)_player->Draw(renderWindow);
	for(auto& it : _enemies)
		it->Draw(renderWindow);
	for (auto& it : _projectiles)
		it->Draw(renderWindow);
	for (auto& it : _pickups)
		it->Draw(renderWindow);
}

void GameWorld::UpdateAll(float deltaTime)
{
	if(_player)_player->Update(deltaTime);
	UpdateEnemySpawning(deltaTime);
	for (auto& it :_enemies)
		it->Update(deltaTime);
	for (auto& it : _projectiles)
		it->Update(deltaTime);
	for (auto& it : _pickups)
		it->Update(deltaTime);
}

void GameWorld::Collision()
{
	if(!_player)return;
	//enemy vs player
	for (auto& it : _enemies)
	{
		if (it->IsExisting() && it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollison(_player.get());
			_player->HandleCollison(it.get());
		}
	}
	//projectile vs player/enemy
	for (auto& it : _projectiles)
	{
		if (it->IsExisting() && (!it->IsFriendly()) && it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollison(_player.get());
			_player->HandleCollison(it.get());
		}
		if (it->IsExisting() && it->IsFriendly())
		{
			for(auto& itEnemy : _enemies)
			{
				if (it->IsExisting() && itEnemy->IsExisting() && it->GetBounds().findIntersection(itEnemy->GetBounds()))
				{
					it->HandleCollison(itEnemy.get());
					itEnemy->HandleCollison(it.get());
				}
			}
		}
	}
	//pickup vs player
	for (auto& it : _pickups)
	{
		if (it->IsExisting() && it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollison(_player.get());
			_player->HandleCollison(it.get());
		}
	}
}

void GameWorld::CleanUp()
{
	auto itEnemy = _enemies.begin();
	while (itEnemy != _enemies.end())
	{
		if ((*itEnemy)->IsExisting())
			++itEnemy;
		else
		{
			auto pickup = std::make_unique<Pickup>(_pickupTexture, (*itEnemy)->GetExperienceValue());
			pickup->SetPosition((*itEnemy)->GetPosition().x, (*itEnemy)->GetPosition().y);
			Add(std::move(pickup));
			itEnemy = _enemies.erase(itEnemy);
		}
	}
	auto itProjectile = _projectiles.begin();
	while (itProjectile != _projectiles.end())
	{
		if ((*itProjectile)->IsExisting())
			++itProjectile;
		else itProjectile = _projectiles.erase(itProjectile);
	}
	auto itPickup = _pickups.begin();
	while (itPickup != _pickups.end())
	{
		if ((*itPickup)->IsExisting())
			++itPickup;
		else itPickup = _pickups.erase(itPickup);
	}
	/*
	//AI:a more efficient way to remove GameObjects
	_enemies.erase(
		std::remove_if(_enemies.begin(), _enemies.end(),
			[](const std::unique_ptr<Enemy>& p) { return !p->IsExisting(); }),
		_enemies.end()
	);
	_projectiles.erase(
		std::remove_if(_projectiles.begin(), _projectiles.end(),
			[](const std::unique_ptr<Projectile>& p) { return !p->IsExisting(); }),
		_projectiles.end()
	);
	_pickups.erase(
		std::remove_if(_pickups.begin(), _pickups.end(),
			[](const std::unique_ptr<Pickup>& p) { return !p->IsExisting(); }),
		_pickups.end()
	);
	*/

}

void GameWorld::UpdateEnemySpawning(float deltaTime)
{
	if (!_player)
		return;

	_elapsedGameTime += deltaTime;
	_enemySpawnTimer += deltaTime;

	float currentSpawnInterval = std::max(_minEnemySpawnInterval, _baseEnemySpawnInterval - _elapsedGameTime * 0.03f);
	if (_enemySpawnTimer < currentSpawnInterval)
		return;

	_enemySpawnTimer = 0.f;

	std::size_t spawnCount = 1 + static_cast<std::size_t>(_elapsedGameTime / 30.f);
	spawnCount = std::min<std::size_t>(spawnCount, 4);

	for (std::size_t i = 0; i < spawnCount && _enemies.size() < _maxEnemyCount; ++i)
	{
		SpawnEnemy(GetRandomEnemySpawnPosition());
	}
}

void GameWorld::SpawnEnemy(sf::Vector2f position)
{
	auto enemy = std::make_unique<Enemy>(_enemyTexture);
	enemy->SetPosition(position.x, position.y);
	enemy->SetTarget(_player.get());
	_enemies.push_back(std::move(enemy));
}

sf::Vector2f GameWorld::GetRandomEnemySpawnPosition()const
{
	int side = RandomInt(0, 3);

	switch (side)
	{
	case 0:
		return { RandomFloat(-EnemySpawnMargin, GameAreaSize.x + EnemySpawnMargin), -EnemySpawnMargin };
	case 1:
		return { RandomFloat(-EnemySpawnMargin, GameAreaSize.x + EnemySpawnMargin), GameAreaSize.y + EnemySpawnMargin };
	case 2:
		return { -EnemySpawnMargin, RandomFloat(-EnemySpawnMargin, GameAreaSize.y + EnemySpawnMargin) };
	default:
		return { GameAreaSize.x + EnemySpawnMargin, RandomFloat(-EnemySpawnMargin, GameAreaSize.y + EnemySpawnMargin) };
	}
}
