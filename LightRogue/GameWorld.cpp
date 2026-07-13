#include"stdafx.h"
#include"GameWorld.h"
#include"GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"
#include"Game.h"

GameWorld::GameWorld()
{
	sf::Image pickupImage({ 16,16 }, sf::Color::Yellow);
	assert(_pickupTexture.loadFromImage(pickupImage));
	sf::Image enemyImage({ 32,32 }, sf::Color::Red);
	assert(_enemyTexture.loadFromImage(enemyImage));
	sf::Image projImage({ 8,8 }, sf::Color(200, 100, 100));
	assert(_enemyProjectileTexture.loadFromImage(projImage));
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

	DrawUI(renderWindow);
}

void GameWorld::UpdateAll(float deltaTime)
{
	if(_player)_player->Update(deltaTime);
	UpdateSpawning(deltaTime);
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
			it->HandleCollision(_player.get());
			_player->HandleCollision(it.get());
		}
	}
	//projectile vs player/enemy
	for (auto& it : _projectiles)
	{
		if (it->IsExisting() && (!it->IsFriendly()) && it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollision(_player.get());
			_player->HandleCollision(it.get());
		}
		if (it->IsExisting() && it->IsFriendly())
		{
			for(auto& itEnemy : _enemies)
			{
				if (it->IsExisting() && itEnemy->IsExisting() && it->GetBounds().findIntersection(itEnemy->GetBounds()))
				{
					it->HandleCollision(itEnemy.get());
					itEnemy->HandleCollision(it.get());
				}
			}
		}
	}
	//pickup vs player
	for (auto& it : _pickups)
	{
		if (it->IsExisting() && it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollision(_player.get());
			_player->HandleCollision(it.get());
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
			Game::GetEventManager().Emit(EnemyKilled{});
			_scoreCounter += (*itEnemy)->GetScoreValue();
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

void GameWorld::UpdateSpawning(float deltaTime)
{
	_gameTime += deltaTime;
	_spawnTimer += deltaTime;
	//120s Giant begin to approach
	if (_gameTime >= 120.f && _gameTime - deltaTime < 120.f)
	{
		auto giant = std::make_unique<Enemy>(_enemyTexture, this, EnemyType::Giant);
		auto pos = GetRandomSpawnPosition();
		giant->SetPosition(pos.x, pos.y);
		giant->SetTarget(_player.get());
		Add(std::move(giant));
	}

	float interval = std::max(_minInterval, _baseInterval - _gameTime * _decayRate);
	if (_spawnTimer < interval)
		return;
	if (_enemies.size() < MAX_ENEMIES_COUNT)
	{
		EnemyType type = EnemyType::Basic;
		if (_gameTime < 30.f)
		{
			type = EnemyType::Basic;
		}
		else if (_gameTime < 60.f)
		{
			float roll = static_cast<float>(rand()) / RAND_MAX;
			if (roll < 0.75f) 
				type = EnemyType::Basic;
			else
				type = EnemyType::Ranged;
		}
		else if(_gameTime<90.f)
		{
			float roll = static_cast<float>(rand()) / RAND_MAX;
			if (roll < 0.60f) 
				type = EnemyType::Basic;
			else if (roll < 0.85f) 
				type = EnemyType::Ranged;
			else
				type = EnemyType::Bomber;
		}
		else if (_gameTime < 150.f)
		{
			float roll = static_cast<float>(rand()) / RAND_MAX;
			if (roll < 0.50f)
				type = EnemyType::Basic;
			else if (roll < 0.80f)
				type = EnemyType::Ranged;
			else
				type = EnemyType::Bomber;
		}
		else if (_gameTime < 210.f)
		{
			float roll = static_cast<float>(rand()) / RAND_MAX;
			if (roll < 0.45f)
				type = EnemyType::Basic;
			else if (roll < 0.75f)
				type = EnemyType::Ranged;
			else if (roll < 0.985f)
				type = EnemyType::Bomber;
			else 
				type = EnemyType::Giant;
		}
		else if (_gameTime < 270.f)
		{
			float roll = static_cast<float>(rand()) / RAND_MAX;
			if (roll < 0.40f)
				type = EnemyType::Basic;
			else if (roll < 0.70f)
				type = EnemyType::Ranged;
			else if (roll < 0.97f)
				type = EnemyType::Bomber;
			else
				type = EnemyType::Giant;
		}
		else //enemy has been enhanced
		{
			float roll = static_cast<float>(rand()) / RAND_MAX;
			if (roll < 0.35f)
				type = EnemyType::Basic;
			else if (roll < 0.70f)
				type = EnemyType::Ranged;
			else if (roll < 0.95f)
				type = EnemyType::Bomber;
			else
				type = EnemyType::Giant;
		}
		auto enemy = std::make_unique<Enemy>(_enemyTexture,this,type);
		auto position = GetRandomSpawnPosition();
		enemy->SetPosition(position.x,position.y);
		enemy->SetTarget(_player.get());
		Add(std::move(enemy));
		_spawnTimer = 0.f;
	}
}

sf::Vector2f GameWorld::GetRandomSpawnPosition()const
{
	const float margin = 40.f;
	const float w = 1024.f, h = 768.f;

	int side = rand() % 4;
	switch (side)
	{
	case 0: return { static_cast<float>(rand()%(int)w), -margin };       // top
	case 1: return { static_cast<float>(rand()%(int)w), h+margin };      // bottom
	case 2: return { -margin, static_cast<float>(rand()%(int)h) };       // left
	case 3: return { w+margin, static_cast<float>(rand()%(int)h) };      // right
	}

	return { w + margin, h / 2.f };
}

void GameWorld::DrawUI(sf::RenderWindow& renderWindow)
{
	_uiManager.DrawPlayerHUD(renderWindow, _player.get());
	/*
	for (auto& enemy : _enemies)
	{
		_uiManager.DrawEnemyHealthBar(renderWindow, enemy.get());
	}*/
}

void GameWorld::DrawUpgradeUI(sf::RenderWindow& renderWindow)
{
	//_uiManager.DrawUpgradeChoices(renderWindow, _player.get());
}

bool GameWorld::IsPlayerDead()const
{
	return _player && !_player->IsExisting();
}

bool GameWorld::IsTimeUp()const
{
	return _gameTime >= _gameTimeLimit;
}

bool GameWorld::HasPendingUpgrade()const
{
	return _player && _player->HasPendingUpgrade();
}

void GameWorld::ApplyUpgradeOption(int option)
{
	if (_player)
		_player->ApplyUpgradeOption(option);
}

float GameWorld::GetGameTime()const
{
	return _gameTime;
}

int GameWorld::GetScore()const
{
	return _scoreCounter;
}

int GameWorld::GetPlayerLevel()const
{
	return _player->GetLevel();
}

const std::array<int, 3>& GameWorld::GetPlayerUpgradeOptions() const
{
	static const std::array<int, 3> fallback = { 1, 2, 3 };
	return _player ? _player->GetCurrentUpgradeOptions() : fallback;
}

const sf::Texture& GameWorld::GetEnemyProjectileTexture() const
{
	return _enemyProjectileTexture;
}

std::string GameWorld::GetUpgradePrompt()const
{
	if (!_player)
		return "";

	const auto& options = _player->GetCurrentUpgradeOptions();
	auto getName = [](int option)
	{
		switch (option)
		{
		case 1: return "extra bullet";
		case 2: return "damage";
		case 3: return "fire rate";
		case 4: return "bullet speed";
		case 5: return "move speed";
		case 6: return "max HP";
		case 7: return "heal";
		case 8: return "less XP need";
		default: return "unknown";
		}
	};

	return "1: " + std::string(getName(options[0])) +
		"  2: " + std::string(getName(options[1])) +
		"  3: " + std::string(getName(options[2]));
}

UIManager& GameWorld::GetUIManager()
{
	return _uiManager;
}
