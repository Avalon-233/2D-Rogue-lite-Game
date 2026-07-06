#pragma once
#include"GameObject.h"

class Projectile :public GameObject
{
public:
	Projectile(const sf::Texture& texture, sf::Vector2f positon,sf::Vector2f speed,float damage, bool isFriendly);
	~Projectile();

	void Update(float deltaTime)override;
	void HandleCollison(Player* player);
	void HandleCollison(Enemy* enemy);

	bool IsFriendly()const;
	float GetDamage()const;

private:
	bool  _isFriendly;
	float _damage;
	sf::Vector2f _speed{};
};