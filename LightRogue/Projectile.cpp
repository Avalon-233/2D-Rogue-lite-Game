#include"stdafx.h"
#include"Projectile.h"
#include"Game.h"

Projectile::Projectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f speed, float damage, bool isFriendly)
    : GameObject(texture), 
	_speed(speed),
    _isFriendly(isFriendly),
    _damage(damage)
{
	_sprite.setOrigin(_sprite.getGlobalBounds().size / 2.f);
	SetPosition(position.x, position.y);
}


Projectile::~Projectile()
{
}

void Projectile::Update(float deltaTime)
{
	_sprite.move(_speed * deltaTime);
	if (GetPosition().x < -50.f || GetPosition().x > 1490.f || GetPosition().y < -50.f || GetPosition().y > 860.f)
		Destroy();
}

void Projectile::HandleCollision(Player* player)
{
	Destroy();
}

void Projectile::HandleCollision(Enemy* enemy)
{
	Destroy();
}

bool Projectile::IsFriendly()const
{
	return _isFriendly;
}

float Projectile::GetDamage()const
{
	return _damage;
}