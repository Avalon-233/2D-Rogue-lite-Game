#pragma once
#include"GameObject.h"

class Player :public GameObject
{
public:
	Player(const sf::Texture& texture);
	~Player();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& renderWindow);
	float GetSpeed()const;

private:
	float _speed;
};