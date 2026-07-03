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
	float GetHP()const;

private:
	float _HP;
	float _maxHP;
	float _speed;
};