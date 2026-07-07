#pragma once
#include"SFML/Window.hpp"
#include"SFML/Graphics.hpp"
#include"Player.h"
#include"GameWorld.h"
#include"Enemy.h"
#include"Projectile.h"
#include"Pickup.h"

class Game
{
public:
	static void Start();

private:
	static bool IsExiting();
	static void GameLoop();

	enum GameState { Uninitialized, ShowingSplash, Paused, ShowingMenu, Playing, Exiting, GameOver, Upgrading };

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	static sf::Clock _clock;
	static GameWorld _gameWorld;

	static void HandleEvent(const sf::Event::Closed&);
	static void HandleEvent(const sf::Event::KeyPressed& event);
	static void HandleEvent(const sf::Event&);
	static void UpdateWindowTitle();
};
