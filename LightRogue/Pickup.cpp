#include"stdafx.h"
#include"Pickup.h"
#include"Game.h"
#include"Player.h"

Pickup::Pickup(const sf::Texture& texture, float experienceValue)
	: GameObject(texture), _experienceValue(experienceValue)
{
	_sprite.setOrigin(_sprite.getGlobalBounds().size / 2.f);
}

Pickup::~Pickup()
{
}

void Pickup::HandleCollision(Player* player)
{
	Destroy();
}

float Pickup::GetExperienceValue()const
{
	return _experienceValue;
}
