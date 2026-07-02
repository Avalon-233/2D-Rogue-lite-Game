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

void Player::Draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(_sprite);
}

float Player::GetSpeed()const
{
	return _speed;
}

void Player::Update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		_sprite.move({0.f, -_speed * deltaTime});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		_sprite.move({0.f, _speed * deltaTime});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
	{
		_sprite.move({-_speed * deltaTime, 0.f});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
	{
		_sprite.move({_speed * deltaTime, 0.f});
	}
