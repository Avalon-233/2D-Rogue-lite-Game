#include"stdafx.h"
#include"GameObject.h"
#include"Game.h"

void Game::HandleEvent(const sf::Event::Closed&)
{
	_gameState = Exiting;
}

void Game::HandleEvent(const sf::Event::KeyPressed& event)
{
	if (event.code == sf::Keyboard::Key::Escape)
	{
		if (_gameState == Playing)
			_gameState = Paused;
		else if (_gameState == Paused)
			_gameState = Playing;
		else if (_gameState == ShowingSplash || _gameState == GameOver)
			_gameState = Exiting;
		return;
	}

	if (event.code == sf::Keyboard::Key::Enter)
	{
		if (_gameState == ShowingSplash)
			_gameState = Playing;
		else if (_gameState == GameOver)
			_gameState = Exiting;
		return;
	}

	if (_gameState == Upgrading)
	{
		if (event.code == sf::Keyboard::Key::Num1 || event.code == sf::Keyboard::Key::Numpad1)
			_gameWorld.ApplyUpgradeOption(1);
		else if (event.code == sf::Keyboard::Key::Num2 || event.code == sf::Keyboard::Key::Numpad2)
			_gameWorld.ApplyUpgradeOption(2);
		else if (event.code == sf::Keyboard::Key::Num3 || event.code == sf::Keyboard::Key::Numpad3)
			_gameWorld.ApplyUpgradeOption(3);

		if (!_gameWorld.HasPendingUpgrade())
			_gameState = Playing;
	}
}

void Game::HandleEvent(const sf::Event&) {}//other events

void Game::Start()
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({ 1024, 768 }, 32), "LightRogue");
	_gameState = Game::ShowingSplash;
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
	if (deltaTime > 0.1f) deltaTime = 0.1f;

	if (_gameState == Playing)
	{
		_gameWorld.UpdateAll(deltaTime);
		_gameWorld.Collision();
		_gameWorld.CleanUp();

		if (_gameWorld.IsPlayerDead() || _gameWorld.IsTimeUp())
			_gameState = GameOver;
		else if (_gameWorld.HasPendingUpgrade())
			_gameState = Upgrading;
	}

	_mainWindow.clear();
	if (_gameState != ShowingSplash)
		_gameWorld.DrawAll(_mainWindow);
	if (_gameState == Upgrading)
		_gameWorld.DrawUpgradeUI(_mainWindow);
	UpdateWindowTitle();
	_mainWindow.display();
}

void Game::UpdateWindowTitle()
{
	switch (_gameState)
	{
	case ShowingSplash:
		_mainWindow.setTitle("LightRogue - Press Enter to start");
		break;
	case Playing:
		_mainWindow.setTitle("LightRogue - Playing | Esc: pause");
		break;
	case Paused:
		_mainWindow.setTitle("LightRogue - Paused | Esc: resume");
		break;
	case Upgrading:
		_mainWindow.setTitle("LightRogue - Upgrade | " + _gameWorld.GetUpgradePrompt());
		break;
	case GameOver:
		_mainWindow.setTitle("LightRogue - Game Over | Enter/Esc: exit");
		break;
	default:
		_mainWindow.setTitle("LightRogue");
		break;
	}
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameWorld Game::_gameWorld;
sf::Clock Game::_clock;
