#pragma once
#include"GameObject.h"

class Player :public GameObject
{
public:
	Player(const sf::Texture& texture);
	~Player();

	void Update(float deltaTime);
	void HandleCollison(Enemy* enemy);
	void HandleCollison(Projectile* projectile);
	void HandleCollison(Pickup* pickup);

	void Shoot();

	float GetSpeed()const;
	float GetHP()const;

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

};