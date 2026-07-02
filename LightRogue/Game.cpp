#include"stdafx.h"
#include"Game.h"

void Game::HandleEvent(const sf::Event::Closed&)
{
	_gameState = Exiting;
}

void Game::HandleEvent(const sf::Event&) {}//other events



void Game::Start(void)
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({ 1024, 768 }, 32), "LightRogue");
	_gameState = Game::Playing;
	_mainWindow.setVerticalSyncEnabled(true);
	_mainWindow.setKeyRepeatEnabled(false);

	Player *player=new Player(sf::Texture());
	_gameWorld.Add("player", player);

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
		/*
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
		/*/
		event->visit([](const auto& type) {HandleEvent(type);});
		//*/
	}

	_gameWorld.UpdateAll(_clock.restart().asSeconds());

	_mainWindow.clear();
	_gameWorld.DrawAll(_mainWindow);
	_mainWindow.display();
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameWorld Game::_gameWorld;
sf::Clock _clock;