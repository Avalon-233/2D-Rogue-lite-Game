#include"stdafx.h"
#include"Enemy.h"
#include"Game.h"
#include"Player.h"
#include"Projectile.h"

Enemy::Enemy(const sf::Texture& texture) :GameObject(texture)
{
	_sprite.setOrigin(_sprite.getGlobalBounds().size / 2.f);
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

	if (length > 0.f)
	{
		direction /= length;
		_sprite.move(direction * _speed * deltaTime);
	}

	if (_visibleTimer.getElapsedTime().asSeconds() > 1.f)
	{
		_isShooted = false;
		_visibleTimer.stop();
	}
}

void Enemy::HandleCollision(Player* player)
{
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

bool Enemy::IsVisible()
{
	auto distance = _target->GetPosition() - GetPosition();
	if (std::sqrt(distance.x * distance.x + distance.y * distance.y) <= _target->GetVision())return true;
	else if (_isShooted)return true;
	else return false;
}