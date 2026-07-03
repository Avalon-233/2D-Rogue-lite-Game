#pragma once
#include"GameObject.h"

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

private:
	std::unique_ptr<Player> _player;
	std::vector<std::unique_ptr<Enemy>> _enemies;
	std::vector<std::unique_ptr<Projectile>> _projectiles;
	std::vector<std::unique_ptr<Pickup>> _pickups;
	
};