#pragma once
#include<string>
#include"SFML\Graphics.hpp"

class Player;
class Enemy;
class Projectile;


class GameObject
{
public:
	GameObject(const sf::Texture& texture);
	virtual ~GameObject();

	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(float deltaTime);
	virtual void HandleCollison(GameObject* gameObject);

	sf::FloatRect GetBounds()const;
	void SetPosition(float x, float y);
	sf::Vector2f GetPosition()const;

	bool IsExisting()const;
	void Destroy();

protected:
	sf::Sprite _sprite;

private:
	sf::Vector2f _position{};
	bool _isExisting = true;

};
