#include"stdafx.h"
#include"GameWorld.h"
#include"GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"
#include"Game.h"
#include"ResourceLoader.h"
#include "SaveManager.h"

GameWorld::GameWorld()
{
	LoadTextureFromResource(_pickupExperienceTexture, "pickup_experience.png");
	LoadTextureFromResource(_pickupHealthTexture, "pickup_health.png");
	LoadTextureFromResource(_enemyBasicTexture, "enemy_basic.png");
	LoadTextureFromResource(_enemyRangedTexture, "enemy_ranged.png");
	LoadTextureFromResource(_enemyBomberTexture, "enemy_bomber.png");
	LoadTextureFromResource(_enemyGiantTexture, "enemy_giant.png");
	LoadTextureFromResource(_enemyProjectileTexture, "bullet_enemy.png");
	LoadTextureFromResource(_explosionTexture, "effect_explosion.png");
	LoadTextureFromResource(_backgroundTexture, "background.png");
}

GameWorld::~GameWorld()
{
}

void GameWorld::SaveGame() const
{
	if (!_player) return;
	SaveData d;
	d.HP = _player->GetHP();
	d.maxHP = _player->GetMaxHP();
	d.position_x = _player->GetPosition().x;
	d.position_y = _player->GetPosition().y;
	d.speed = _player->GetSpeed();
	d.shootDamage = _player->GetShootDamage();
	d.shootSpeed = _player->GetShootSpeed();
	d.shootCooldown = _player->GetShootCooldown();
	d.projectileCount = _player->GetProjectileCount();
	d.experience = _player->GetExperience();
	d.experienceNeedMultiplier = _player->GetExperienceNeedMultiplier();
	d.level = _player->GetLevel();
	d.pendingUpgradeCount = _player->GetPendingUpgradeCount();
	d.gameTime = _gameTime;
	d.score = _scoreCounter;
	SaveManager::Save(d);
}

bool GameWorld::LoadGame()
{
	SaveData d;
	if (!SaveManager::Load(d)) return false;
	if (!_player) return false;
	_player->ApplySaveData(d);
	_gameTime = d.gameTime;
	_scoreCounter = d.score;
	return true;
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
	sf::Sprite _background(_backgroundTexture);
	renderWindow.draw(_background);
	if (_player)_player->Draw(renderWindow);
	sf::CircleShape Vision(_player->GetVision(),200);
	Vision.setOrigin({ _player->GetVision(), _player->GetVision() });
	Vision.setPosition(_player->GetPosition());
	Vision.setFillColor(sf::Color(255, 255, 255,32));
	renderWindow.draw(Vision);

	for(auto& it : _enemies)
		it->Draw(renderWindow);
	for (auto& it : _projectiles)
		it->Draw(renderWindow);
	for (auto& it : _pickups)
		it->Draw(renderWindow);
	for (auto& effect : _effects)
		renderWindow.draw(effect.sprite);

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
	for (auto it = _effects.begin(); it != _effects.end();)
	{
		it->remaining -= deltaTime;
		if (it->remaining <= 0.f)
			it = _effects.erase(it);
		else
			++it;
	}
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
			const sf::Vector2f deathPosition = (*itEnemy)->GetPosition();
			_effects.emplace_back(_explosionTexture, deathPosition);
			const sf::Texture& pickupTexture = (*itEnemy)->GetHealValue() > 0.f
				? _pickupHealthTexture : _pickupExperienceTexture;
			auto pickup = std::make_unique<Pickup>(pickupTexture, (*itEnemy)->GetExperienceValue(), (*itEnemy)->GetHealValue());
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
		auto giant = std::make_unique<Enemy>(GetEnemyTexture(EnemyType::Giant), this, EnemyType::Giant);
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
				type = EnemyType::Elite;
			else if (roll < 0.70f)
				type = EnemyType::Ranged;
			else if (roll < 0.95f)
				type = EnemyType::Bomber;
			else
				type = EnemyType::Giant;
		}
		auto enemy = std::make_unique<Enemy>(GetEnemyTexture(type),this,type);
		auto position = GetRandomSpawnPosition();
		enemy->SetPosition(position.x,position.y);
		enemy->SetTarget(_player.get());
		Add(std::move(enemy));
		_spawnTimer = 0.f;
	}
}

const sf::Texture& GameWorld::GetEnemyTexture(EnemyType type)const
{
	switch (type)
	{
	case EnemyType::Ranged: return _enemyRangedTexture;
	case EnemyType::Bomber: return _enemyBomberTexture;
	case EnemyType::Giant:  return _enemyGiantTexture;
	case EnemyType::Basic:
	case EnemyType::Elite:
	default:                return _enemyBasicTexture;
	}
}

sf::Vector2f GameWorld::GetRandomSpawnPosition()const
{
	const float margin = 40.f;
	const float w = 1440.f, h = 810.f;

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

SoundManager& GameWorld::GetSoundManager()
{ 
	return _soundManager;
}
