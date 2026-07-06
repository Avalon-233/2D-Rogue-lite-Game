#pragma once
#include"GameObject.h"

class Enemy;
class Projectile;
class Pickup;

class Player :public GameObject
{
public:
	Player(const sf::Texture& texture);
	~Player();

	void Update(float deltaTime)override;
	void HandleCollison(Enemy* enemy);
	void HandleCollison(Projectile* projectile);
	void HandleCollison(Pickup* pickup);

	void Shoot();
	void AddExperience(float experience);

	float GetSpeed()const;
	float GetHP()const;
	float GetExperience()const;
	int GetLevel()const;

private:
	//basic
	float _HP=100;
	float _maxHP=100;
	float _speed=10;
	//shoot-about
	float _shootCooldown=0.5f;
	float _shootTimer=0.f;
	//levevl-about
	float _experience=0.f;
	int _level=1;
	//other

	void CheckLevelUp();

};
