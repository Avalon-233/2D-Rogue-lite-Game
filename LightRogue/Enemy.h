#pragma once
#include"GameObject.h"

class Player;
class Projectile;

class Enemy :public GameObject
{
public:
	Enemy(const sf::Texture& texture);
	~Enemy();

	void Update(float deltaTime)override;
	void HandleCollison(Player* player);
	void HandleCollison(Projectile* projectile);

	void SetTarget(Player* player);

	float GetHP()const;
	float GetMaxHP()const;
	float GetDamage()const;
	float GetExperienceValue()const;

private:
	Player* _target = nullptr;
	float _maxHP = 30.f;
	float _HP = 30.f;
	float _damage = 10.f;
	float _speed = 60.f;
	float _experienceValue = 5.f;

};
