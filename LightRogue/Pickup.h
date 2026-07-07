#pragma once
#include"GameObject.h"

class Player;

class Pickup :public GameObject
{
public:
	Pickup(const sf::Texture& texture, float experienceValue);
	~Pickup();

	void HandleCollison(Player* player);

	float GetExperienceValue()const;

private:
	float _experienceValue = 0.f;

};
