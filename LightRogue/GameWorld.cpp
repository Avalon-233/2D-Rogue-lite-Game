#include"stdafx.h"
#include"GameWorld.h"
#include"GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"

GameWorld::GameWorld()
{
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
		if (it->IsExisting() /*&& itProjectile->GetOwner() == enemy )*/ && it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollison(_player.get());
			_player->HandleCollison(it.get());
		}
		if (it->IsExisting() /*&& itProjectile->GetOwner() == player*/)
		{
			for(auto& itEnemy : _enemies)
			{
				if (it->GetBounds().findIntersection(itEnemy->GetBounds()))
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
		if (it->GetBounds().findIntersection(_player->GetBounds()))
		{
			it->HandleCollision(_player.get());
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
		else itEnemy = _enemies.erase(itEnemy);
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
