#include"stdafx.h"
#include"Player.h"
#include"Game.h"
#include"Enemy.h"
#include"Projectile.h"
#include"Pickup.h"

Player::Player(const sf::Texture& texture,GameWorld* gameWorld, sf::RenderWindow* mainWindow) :GameObject(texture),_gameWorld(gameWorld),_mainWindow(mainWindow)
{
	_sprite.setOrigin(_sprite.getGlobalBounds().size/2.f);
	sf::Image projectileImage({ 8,8 }, sf::Color::Cyan);
	assert(_projectileTexture.loadFromImage(projectileImage));
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	//move
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		_sprite.move({ 0.f, -_speed * deltaTime });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		_sprite.move({ 0.f, _speed * deltaTime });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
	{
		_sprite.move({ -_speed * deltaTime, 0.f });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
	{
		_sprite.move({ _speed * deltaTime, 0.f });
	}
	//check HP
	if (_HP <= 0.f)
		Destroy();
	if (_HP > _maxHP)
		_HP = _maxHP;
	//shoot
	_shootTimer += deltaTime;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (_shootTimer >= _shootCooldown)
		{
			Shoot(sf::Mouse::getPosition(*_mainWindow));
			_shootTimer=0.f;
		}
	}
}

void Player::HandleCollision(Enemy* enemy)
{
	_HP -= enemy->GetDamage();
	Game::GetEventManager().Emit(PlayerDamaged{});
}

void Player::HandleCollision(Projectile* projectile)
{
	
	_HP-= projectile->GetDamage();
	Game::GetEventManager().Emit(PlayerDamaged{});
	
}

void Player::HandleCollision(Pickup* pickup)
{
	AddExperience(pickup->GetExperienceValue());
}

void Player::Shoot(sf::Vector2i position)
{
	//create a projectile and Add() it
	sf::Vector2f distance = { position.x*1.f - GetPosition().x,position.y*1.f - GetPosition().y };
	if (distance.x ==0.&&distance.y== 0.f)return;
	sf::Vector2f direction = distance.normalized();
	if (_projectileCount <= 1)
	{
		FireProjectile(direction);
		return;
	}

	float spreadStep = 10.f * 3.1415926f / 180.f;
	float startAngle = -spreadStep * static_cast<float>(_projectileCount - 1) / 2.f;
	for (int i = 0; i < _projectileCount; ++i)
	{
		float angle = startAngle + spreadStep * static_cast<float>(i);
		sf::Vector2f spreadDirection(
			direction.x * std::cos(angle) - direction.y * std::sin(angle),
			direction.x * std::sin(angle) + direction.y * std::cos(angle)
		);
		FireProjectile(spreadDirection);
	}
}

void Player::AddExperience(float experience)
{
	_experience += experience;
	CheckLevelUp();
}

float Player::GetVision()const
{
	return _visionRadius;
}

float Player::GetSpeed()const
{
	return _speed;
}

float Player::GetHP()const
{
	return _HP;
}

float Player::GetMaxHP()const
{
	return _maxHP;
}

float Player::GetExperience()const
{
	return _experience;
}

float Player::GetExperienceNeeded()const
{
	return _level * 10.f * _experienceNeedMultiplier;
}

int Player::GetLevel()const
{
	return _level;
}

bool Player::HasPendingUpgrade()const
{
	return _pendingUpgradeCount > 0;
}

void Player::ApplyUpgradeOption(int option)
{
	if (_pendingUpgradeCount <= 0)
		return;
	if (option < 1 || option > 3)
		return;

	ApplyUpgradeType(static_cast<UpgradeType>(_currentUpgradeOptions[option - 1]));
	_pendingUpgradeCount--;

	if (_pendingUpgradeCount > 0)
		RollUpgradeOptions();
}

const std::array<int, 3>& Player::GetCurrentUpgradeOptions()const
{
	return _currentUpgradeOptions;
}

void Player::CheckLevelUp()
{
	float experienceNeeded = GetExperienceNeeded();
	while (_experience >= experienceNeeded)
	{
		_experience -= experienceNeeded;
		_level++;
		if (_pendingUpgradeCount == 0)
			RollUpgradeOptions();
		_pendingUpgradeCount++;
		experienceNeeded = GetExperienceNeeded();
		Game::GetEventManager().Emit(LevelUp{ _level});
	}
}

void Player::FireProjectile(sf::Vector2f direction)
{
	auto projectile = std::make_unique<Projectile>(_projectileTexture, GetPosition(), direction * _shootSpeed, _shootDamage, true);
	_gameWorld->Add(std::move(projectile));
}

void Player::RollUpgradeOptions()
{
	std::vector<int> upgradePool{
		ExtraProjectile,
		DamageUp,
		FireRateUp,
		ProjectileSpeedUp,
		MoveSpeedUp,
		MaxHPUp,
		HealUp,
		ExperienceNeedDown
	};

	for (std::size_t i = 0; i < _currentUpgradeOptions.size(); ++i)
	{
		int index = rand() % static_cast<int>(upgradePool.size());
		_currentUpgradeOptions[i] = upgradePool[index];
		upgradePool.erase(upgradePool.begin() + index);
	}
}

void Player::ApplyUpgradeType(UpgradeType upgradeType)
{
	switch (upgradeType)
	{
	case ExtraProjectile:
		_projectileCount++;
		break;
	case DamageUp:
		_shootDamage += 5.f;
		break;
	case FireRateUp:
		if (_shootCooldown > 0.12f)
			_shootCooldown -= 0.08f;
		break;
	case ProjectileSpeedUp:
		_shootSpeed += 80.f;
		break;
	case MoveSpeedUp:
		_speed += 20.f;
		break;
	case MaxHPUp:
		_maxHP += 20.f;
		_HP += 20.f;
		if (_HP > _maxHP)
			_HP = _maxHP;
		break;
	case HealUp:
		_HP += 35.f;
		if (_HP > _maxHP)
			_HP = _maxHP;
		break;
	case ExperienceNeedDown:
		if (_experienceNeedMultiplier > 0.55f)
			_experienceNeedMultiplier -= 0.1f;
		break;
	default:
		break;
	}
}
