#pragma once
#include"GameObject.h"
#include"GameWorld.h"

#include <array>

class Enemy;
class Projectile;
class Pickup;

class Player :public GameObject
{
public:
	Player(const sf::Texture& texture,GameWorld* gameWorld, sf::RenderWindow* mainWindow);
	~Player();

	bool IsVisible() override;
	void Update(float deltaTime)override;
	void HandleCollision(Enemy* enemy);
	void HandleCollision(Projectile* projectile);
	void HandleCollision(Pickup* pickup);

	void Shoot(sf::Vector2i position);
	void AddExperience(float experience);

	float GetVision()const;
	float GetSpeed()const;
	float GetHP()const;
	float GetMaxHP()const;
	float GetExperience()const;
	float GetExperienceNeeded()const;
	int GetLevel()const;
	bool HasPendingUpgrade()const;
	void ApplyUpgradeOption(int option);
	const std::array<int, 3>& GetCurrentUpgradeOptions()const;

private:
	enum UpgradeType
	{
		ExtraProjectile = 1,
		DamageUp,
		FireRateUp,
		ProjectileSpeedUp,
		MoveSpeedUp,
		MaxHPUp,
		HealUp,
		ExperienceNeedDown
	};

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
	int _projectileCount=1;
	sf::Texture _projectileTexture;
	//levevl-about
	float _experience=0.f;
	float _experienceNeedMultiplier=1.f;
	int _level=1;
	int _pendingUpgradeCount=0;
	std::array<int, 3> _currentUpgradeOptions{ ExtraProjectile, DamageUp, FireRateUp };
	
	void CheckLevelUp();
	void RollUpgradeOptions();
	void ApplyUpgradeType(UpgradeType upgradeType);

	//vision about
	float _visionRadius = 300.f;

	//invincible about
	float _invincibleTimer = 0.f;
	float _invincibleDuration = 1.2f;
	float _flashTimer = 0.f;
	float _flashInterval = 0.1f;
	bool  _flashVisible = true;
};
