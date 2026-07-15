#pragma once
#include"GameObject.h"

class Player;

class Pickup :public GameObject
{
public:
	Pickup(const sf::Texture& texture, float experienceValue, float healValue);
	~Pickup();

	void HandleCollision(Player* player);

	float GetExperienceValue()const;
	float GetHealValue()const;

private:
	float _experienceValue = 0.f;
	float _healValue=0.f;

};
