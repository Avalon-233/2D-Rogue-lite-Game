#include"Stdafx.h"
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

void GameObject::SetPosition(float x, float y)
{
	_sprite.setPosition({x,y});
}

void GameObject::Update(float deltaTime)
{

}

sf::Vector2f GameObject::GetPosition()const
{
	return _sprite.getPosition();
}