#pragma once
#include"GameObject.h"

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Add(std::string name, GameObject* gameObject);
	void Remove(std::string name);
	int GetCount() const;
	GameObject* Get(std::string name) const;

	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll(float deltaTime);

private:
	std::map<std::string, GameObject*> _gameObjects;

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string,GameObject*>& p) const
		{
			delete p.second;
		}
	};

};