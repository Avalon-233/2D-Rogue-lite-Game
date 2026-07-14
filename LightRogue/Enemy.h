#pragma once
#include"GameObject.h"

class Player;
class Projectile;
class GameWorld;

enum EnemyType { Basic,Elite, Ranged, Bomber, Giant };

class Enemy :public GameObject
{
public:
	Enemy(const sf::Texture& texture, GameWorld* gameWorld, EnemyType type);
	~Enemy();

	void Update(float deltaTime)override;
	void HandleCollision(Player* player);
	void HandleCollision(Projectile* projectile);

	void SetTarget(Player* player);

	float GetHP()const;
	float GetMaxHP()const;
	float GetDamage()const;
	float GetExperienceValue()const;
	int GetScoreValue()const;
	bool IsVisible()override;

private:
	EnemyType _type;
	//basic properties
	Player* _target = nullptr;
	GameWorld* _gameWorld;
	float _maxHP = 30.f;
	float _HP = 30.f;
	float _damage = 10.f;
	float _speed = 60.f;
	float _experienceValue = 5.f;
	int _scoreValue = 1;
	//ranged-specific
	float _shootRange = 250.f;
	float _escapeDistance = 180.f;
	float _shootCooldown = 0.5f;
	float _shootTimer = 0.f;
	float _shootDamage = 10.f;
	float _shootSpeed = 500.f;
	void Shoot();
	//bomber-specific
	float _bombRadius = 30.f;
	void Bomb();

	bool _isShooted=false;
	sf::Clock _visibleTimer;
};
