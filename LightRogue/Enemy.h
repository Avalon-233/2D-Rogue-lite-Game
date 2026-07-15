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
	float GetHealValue()const;
	bool IsVisible()override;

private:
	EnemyType _type;
	//basic properties
	Player* _target = nullptr;
	GameWorld* _gameWorld;
	float _maxHP = 0.f;
	float _HP = 0.f;
	float _damage = 0.f;
	float _speed = 0.f;
	float _experienceValue = 0.f;
	int _scoreValue = 0;
	float _healValue = 0.f;
	//ranged-specific
	float _shootRange = 0.f;
	float _escapeDistance = 0.f;
	float _shootCooldown = 0.f;
	float _shootTimer = 0.f;
	float _shootDamage = 0.f;
	float _shootSpeed = 0.f;
	void Shoot();
	//bomber-specific
	float _bombRadius = 0.f;
	void Bomb();

	bool _isShooted=false;
	sf::Clock _visibleTimer;
};
