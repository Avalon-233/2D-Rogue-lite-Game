#include"stdafx.h"
#include"GameObject.h"
#include"Game.h"

void Game::HandleEvent(const sf::Event::Closed&)
{
	_gameState = Exiting;
}

void Game::HandleEvent(const sf::Event&) {}//other events



void Game::Start()
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({ 1024, 768 }, 32), "LightRogue");
	_gameState = Game::Playing;
	_mainWindow.setVerticalSyncEnabled(true);
	_mainWindow.setKeyRepeatEnabled(false);
	_clock.restart();
	
	//Create a player
	sf::Image image({ 32,32 }, sf::Color::Green);
	sf::Texture playerTexture(image);
	std::unique_ptr<Player> player = std::make_unique<Player>(playerTexture,&_gameWorld,&_mainWindow);
	player->SetPosition(512.f, 384.f);
	_gameWorld.Add(std::move(player));

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
		event->visit([](const auto& type) {HandleEvent(type);});
	}

	float deltaTime = _clock.restart().asSeconds();
	if (deltaTime>0.1f) deltaTime = 0.1f;
	_gameWorld.UpdateAll(deltaTime);
	_gameWorld.Collision();
	_gameWorld.CleanUp();

	_mainWindow.clear();
	_gameWorld.DrawAll(_mainWindow);
	_mainWindow.display();
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameWorld Game::_gameWorld;
sf::Clock Game::_clock;
