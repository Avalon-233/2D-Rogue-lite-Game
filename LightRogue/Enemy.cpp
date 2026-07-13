#include"stdafx.h"
#include"Enemy.h"
#include"Game.h"
#include"Player.h"
#include"Projectile.h"

Enemy::Enemy(const sf::Texture& texture, GameWorld* gameWorld, EnemyType type) :GameObject(texture),_gameWorld(gameWorld),_type(type)
{
	_sprite.setOrigin(_sprite.getGlobalBounds().size / 2.f);

	switch (_type)
	{
	case EnemyType::Basic:
		_maxHP = 40.f;
		_HP = 40.f;
		_damage = 10.f;
		_speed = 60.f;
		_experienceValue = 5.f;
		_scoreValue = 1;
		break;
	case EnemyType::Ranged:
		_maxHP = 25.f;
		_HP = 25.f;
		_damage = 5.f;
		_speed = 50.f;
		_experienceValue = 8.f;
		_scoreValue = 2;
		_shootRange = 250.f;
		_escapeDistance = 180.f;
		_shootCooldown = 1.5f;
		_shootDamage = 10.f;
		_shootSpeed = 250.f;
		_sprite.setColor(sf::Color(100, 150, 255));
		break;
	case EnemyType::Bomber:
		_maxHP = 15.f;
		_HP = 15.f;
		_damage = 30.f;
		_speed = 100.f;
		_experienceValue = 10.f;
		_scoreValue = 3;
		_bombRadius = 30.f;
		_sprite.setColor(sf::Color(255, 150, 50));
		break;
	case EnemyType::Giant:
		_maxHP = 500.f;
		_HP = 500.f;
		_damage = 45.f;
		_speed = 35.f;
		_experienceValue = 30.f;
		_scoreValue = 10;
		_sprite.setScale({ 3.f, 3.f });
		_sprite.setColor(sf::Color(200, 80, 200));
	}
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	if (!_target)
		return;

	sf::Vector2f direction = _target->GetPosition() - GetPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	switch (_type)
	{
	case EnemyType::Basic:
		if (length > 0.f)
		{
			direction /= length;
			_sprite.move(direction * _speed * deltaTime);
		}
		break;
	case EnemyType::Ranged:
		if (length > 0.f)
		{
			direction /= length;
			if (length > _shootRange)
				_sprite.move(direction * _speed * deltaTime);
			else if (length < _escapeDistance)
				_sprite.move(-direction * _speed * deltaTime);
		}
		_shootTimer += deltaTime;
		if (_shootTimer >= _shootCooldown && length <= _shootRange)
		{
			Shoot();
			_shootTimer = 0.f;
		}
		break;
	case EnemyType::Bomber:
		if (length > 0.f)
		{
			direction /= length;
			_sprite.move(direction * _speed * deltaTime);
		}
		break;
	}

	if (_visibleTimer.getElapsedTime().asSeconds() > 1.f)
	{
		_isShooted = false;
		_visibleTimer.stop();
	}
}

void Enemy::HandleCollision(Player* player)
{
	if (_type == EnemyType::Bomber)
	{
		Destroy();
	}
}

void Enemy::HandleCollision(Projectile* projectile)
{
	_isShooted = true;
	_visibleTimer.restart();
	_HP -= projectile->GetDamage();
	if (_HP <= 0.f)
		Destroy();
}

void Enemy::SetTarget(Player* player)
{
	_target = player;
}

float Enemy::GetHP()const
{
	return _HP;
}

float Enemy::GetMaxHP()const
{
	return _maxHP;
}

float Enemy::GetDamage()const
{
	return _damage;
}

float Enemy::GetExperienceValue()const
{
	return _experienceValue;
}
int Enemy::GetScoreValue()const
{
	return _scoreValue;
}

bool Enemy::IsVisible()
{
	auto distance = _target->GetPosition() - GetPosition();
	if (std::sqrt(distance.x * distance.x + distance.y * distance.y) <= _target->GetVision())return true;
	else if (_isShooted)return true;
	else return false;
}

void Enemy::Shoot()
{
	sf::Vector2f distance = _target->GetPosition() - GetPosition();
	float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);
	if (length == 0.f) return;
	sf::Vector2f direction = distance.normalized();

	auto projectile = std::make_unique<Projectile>(_gameWorld->GetEnemyProjectileTexture(), GetPosition(), direction * _shootSpeed, _shootDamage, false);
	_gameWorld->Add(std::move(projectile));
}

void Enemy::Bomb()
{

}