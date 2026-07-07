#pragma once
#include"GameObject.h"
#include"GameWorld.h"

class Enemy;
class Projectile;
class Pickup;

class Player :public GameObject
{
public:
	Player(const sf::Texture& texture,GameWorld* gameWorld, sf::RenderWindow* mainWindow);
	~Player();

	void Update(float deltaTime)override;
	void HandleCollison(Enemy* enemy);
	void HandleCollison(Projectile* projectile);
	void HandleCollison(Pickup* pickup);

	void Shoot(sf::Vector2i position);
	void AddExperience(float experience);

	float GetSpeed()const;
	float GetHP()const;
	float GetMaxHP()const;
	float GetExperience()const;
	float GetExperienceNeeded()const;
	int GetLevel()const;

private:

	sf::RenderWindow* _mainWindow;
	GameWorld* _gameWorld ;

	//basic
	float _HP=100;
	float _maxHP=100;
	float _speed=100;
	//shoot-about
	float _shootCooldown=0.5f;
	float _shootTimer=0.f;
	float _shootDamage=10.f;
	float _shootSpeed=500.f;
	sf::Texture _projectileTexture;
	//levevl-about
	float _experience=0.f;
	int _level=1;
	//other

	void CheckLevelUp();

};
