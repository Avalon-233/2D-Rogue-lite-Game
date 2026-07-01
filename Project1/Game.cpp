#include"stdafx.h"
#include"Game.h"

void HandleEvent(const sf::Event&) {}//other events

void Game::Start(void)
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({ 1024, 768 }, 32), "Project1");
	_gameState = Game::Playing;
	_mainWindow.setVerticalSyncEnabled(true);
	_mainWindow.setKeyRepeatEnabled(false);

	while (!IsExiting())
	{
		GameLoop();
	}

	_mainWindow.close();
}

bool Game::IsExiting()
{
	return _gameState == Game::Exiting ? true : false;
}

void Game::GameLoop()
{
	while (const std::optional event = Game::_mainWindow.pollEvent())
	{
		
		switch (_gameState)
		{
			case Game::Playing:
			{
				_mainWindow.clear(sf::Color(0,0,255));
				_mainWindow.display();

				if (event->is<sf::Event::Closed>())
				{
					_gameState = Game::Exiting;
				}
				event->visit([](const auto& type) {});
				break;
			}
		}
		/*
		if (event->is<sf::Event::Closed>())
		{
			_gameState = Game::Exiting;
		}

		event->visit([](const auto& type) {HandleEvent(type);});
		*/
	}

	//Update

	_mainWindow.clear();
	//_mainWindow.draw();
	_mainWindow.display();
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
