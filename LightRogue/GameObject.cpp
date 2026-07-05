#include"stdafx.h"
#include"GameObject.h"

GameObject::GameObject(const sf::Texture& texture):_sprite(texture)
{
}

GameObject::~GameObject()
{
}

void GameObject::Draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(_sprite);
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::HandleCollision(GameObject* gameObject)
{
}

sf::FloatRect GameObject::GetBounds()const
{
	return _sprite.getGlobalBounds();
}

void GameObject::SetPosition(float x, float y)
{
	_sprite.setPosition({x,y});
}

sf::Vector2f GameObject::GetPosition()const
{
	return _sprite.getPosition();
}

bool GameObject::IsExisting()const
{
	return _isExisting;
}

void GameObject::Destroy()
{
	_isExisting = false;
}