#include"stdafx.h"
#include"Player.h"
#include"Game.h"

Player::Player(const sf::Texture& texture) :GameObject(texture)
{
	_sprite.setOrigin(_sprite.getGlobalBounds().size/2.f);
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
			Shoot();
			_shootTimer=0.f;
		}
	}
	//level up
	if (_experience >= _level * 10)
	{
		_level++;
		_experience-= _level * 10;
		//boost stats
	}
}

void Player::HandleCollison(Enemy* enemy)
{
	/*
	_HP -= enemy->damage;
	*/
}

void Player::HandleCollison(Projectile* projectile)
{
	
	_HP-= projectile->GetDamage();
	
}

void Player::HandleCollison(Pickup* pickup)
{
	/*
	_experience += pickup->experienceUp;
	*/
}

void Player::Shoot()
{
	//create a projectile and Add() it
}

float Player::GetSpeed()const
{
	return _speed;
}

float Player::GetHP()const
{
	return _HP;
}