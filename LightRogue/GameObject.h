#pragma once
#include<string>
#include"SFML\Graphics.hpp"

class GameObject
{
public:
	GameObject(const sf::Texture& texture);
	virtual ~GameObject();

	virtual void Draw(sf::RenderWindow& window);
	virtual void SetPosition(float x, float y);

	virtual void Update(float deltaTime);
	virtual sf::Vector2f GetPosition()const;

protected:
	sf::Sprite _sprite;
	
	//sf::Sprite& Getsprite();

};