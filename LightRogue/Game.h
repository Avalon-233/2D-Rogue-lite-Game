#pragma once
#include"SFML/Window.hpp"
#include"SFML/Graphics.hpp"
#include"Player.h"
#include"GameWorld.h"

class Game
{
public:
	static void Start();

private:
	static bool IsExiting();
	static void GameLoop();
	//static void Update();
	//static void Render();

	enum GameState { Uninitialized, ShowingSplash, Paused, ShowingMenu, Playing, Exiting };

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	static sf::Clock _clock;
	static GameWorld _gameWorld;

	static void HandleEvent(const sf::Event::Closed&);
	static void HandleEvent(const sf::Event&);
};