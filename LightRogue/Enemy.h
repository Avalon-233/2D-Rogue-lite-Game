#pragma once
#include"GameObject.h"

class Player;
class Projectile;

class Enemy :public GameObject
{
public:
	Enemy(const sf::Texture& texture);
	~Enemy();

	bool IsVisible()override;
	void Update(float deltaTime)override;
	void HandleCollision(Player* player);
	void HandleCollision(Projectile* projectile);

	void SetTarget(Player* player);

	float GetHP()const;
	float GetMaxHP()const;
	float GetDamage()const;
	float GetExperienceValue()const;
	int GetScoreValue()const;

private:
	Player* _target = nullptr;
	float _maxHP = 30.f;
	float _HP = 30.f;
	float _damage = 10.f;
	float _speed = 60.f;
	float _experienceValue = 5.f;
	int _scoreValue = 1;

	bool _isShooted=false;

	bool IsViSible();

	sf::Clock _visibleTimer;
};
