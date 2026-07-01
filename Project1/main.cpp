#include"stdafx.h"
#include"Game.h"

void Game::Start(void)
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({1024, 768},32), "Project1");
	_gameState = Game::Playing;

	while (!IsExiting())
	{
		GameLoop();
	}

	_mainWindow.close();
}

bool Game::IsExiting()
{
	if (_gameState == Game::Exiting)
		return 1;
	else
		return 0;
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
				break;
			}
		}
	}
	_mainWindow.clear();

	_mainWindow.display();
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;

int main(int argc,char** argv)
{
	Game::Start();

	return 0;
}