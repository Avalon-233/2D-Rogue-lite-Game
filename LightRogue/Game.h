#pragma once
#include"SFML/Window.hpp"
#include"SFML/Graphics.hpp"
#include"Player.h"
#include"GameWorld.h"
#include"Enemy.h"
#include"Projectile.h"
#include"Pickup.h"
#include"GameEvent.h"
#include"UI.h"

class Game
{
public:
	static void Start();
	static EventManager& GetEventManager();

private:
	static bool IsExiting();
	static void GameLoop();
	static sf::Vector2i ToWorld(sf::Vector2i pixel);

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	static sf::Clock _clock;
	static GameWorld _gameWorld;
	static EventManager _eventManager;
	static sf::View _view;

	static void BeginPlaying();

	static void HandleEvent(const sf::Event::Closed&);
	static void HandleEvent(const sf::Event::MouseMoved& event);
	static void HandleEvent(const sf::Event::MouseButtonPressed& event);
	static void HandleEvent(const sf::Event::KeyPressed& event);
	static void HandleEvent(const sf::Event::Resized&);
	static void HandleEvent(const sf::Event&);
	static void ProcessEvents();

	static void UpdateWindowTitle();
};
