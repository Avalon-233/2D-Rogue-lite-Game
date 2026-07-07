#include"stdafx.h"
#include"GameWorld.h"
#include"GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"

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
	float interval = std::max(_minInterval, _baseInterval - _gameTime * _decayRate);
	if (_spawnTimer < interval)
		return;
	if (_enemies.size() < MAX_ENEMIES_COUNT)
	{
		auto enemy = std::make_unique<Enemy>(_enemyTexture);
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
	case 0: return { static_cast<float>(rand()%(int)w), -margin };          // 上
	case 1: return { static_cast<float>(rand()%(int)w), h+margin };       // 下
	case 2: return { -margin, static_cast<float>(rand()%(int)h) };          // 左
	case 3: return { w+margin, static_cast<float>(rand()%(int)h) };       // 右
	}

}

void GameWorld::DrawUI(sf::RenderWindow& renderWindow)
{
	_uiManager.DrawPlayerHUD(renderWindow, _player.get());

	for (auto& enemy : _enemies)
	{
		_uiManager.DrawEnemyHealthBar(renderWindow, enemy.get());
	}
}
