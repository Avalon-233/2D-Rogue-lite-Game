#include"stdafx.h"
#include"GameWorld.h"

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
	std::for_each(_gameObjects.begin(), _gameObjects.end(), GameObjectDeallocator());
}

void GameWorld::Add(std::string name, GameObject* gameObject)
{
	_gameObjects.insert(std::pair<std::string, GameObject*>(name,gameObject));
}
void GameWorld::Remove(std::string name)
{
	auto it = _gameObjects.find(name);
	if (it != _gameObjects.end())
	{
		delete it->second;
		_gameObjects.erase(it);
	}
}

GameObject* GameWorld::Get(std::string name) const
{
	auto it = _gameObjects.find(name);
	if (it != _gameObjects.end())
	{
		return it->second;
	}
	return nullptr;
}

int GameWorld::GetCount()const
{
	return _gameObjects.size();
}

void GameWorld::DrawAll(sf::RenderWindow& renderWindow)
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		it->second->Draw(renderWindow);
	}
	/*
	for (auto it : _gameObjects)
	{
		it->second->Draw(renderWindow);
	}
	*/
}

void GameWorld::UpdateAll(float deltaTime)
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		it->second->Update(deltaTime);
	}
	/*
	for (auto it : _gameObjects)
	{
		it->second->Update(deltaTime);
	}
	*/
}