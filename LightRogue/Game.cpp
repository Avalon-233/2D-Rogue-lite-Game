#include"stdafx.h"
#include"GameObject.h"
#include"Game.h"

void Game::HandleEvent(const sf::Event::Closed&)
{
	_eventManager.Emit(GameStateChange{ Exiting });
}

void Game::HandleEvent(const sf::Event::MouseMoved& event)
{
	_gameWorld.GetUIManager().UpdateOverlayHover(_mainWindow, event.position);
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed& event)
{
	int action = _gameWorld.GetUIManager().HitTestOverlay(_mainWindow, event.position);

	switch (_gameState)
	{
	case ShowingSplash:
		if (action == 1) BeginPlaying();
		break;
	case Paused:
		if (action == 1) _eventManager.Emit(GameStateChange{ Playing });
		if (action == 2) _eventManager.Emit(GameStateChange{ Exiting });
		break;
	case Upgrading:
		if (action >= 1 && action <= 3)
			_eventManager.Emit(UpgradeSelected{ action });
		break;
	}
}

void Game::HandleEvent(const sf::Event::KeyPressed& event)
{
	switch (_gameState)
	{
	case Playing:
		if (event.code == sf::Keyboard::Key::Escape)
			_eventManager.Emit(GameStateChange{ Paused });
		break;
	case Paused:
		if (event.code == sf::Keyboard::Key::Escape)
			_eventManager.Emit(GameStateChange{ Playing });
		break;
	case GameOver:
		_eventManager.Emit(GameStateChange{ Exiting });
		break;
	case ShowingSplash:
		if (event.code == sf::Keyboard::Key::Escape)
			_eventManager.Emit(GameStateChange{ Exiting });
		break;
	case Upgrading:
		if (event.code == sf::Keyboard::Key::Num1 || event.code == sf::Keyboard::Key::Numpad1)
			_eventManager.Emit(UpgradeSelected{ 1 });
		else if (event.code == sf::Keyboard::Key::Num2 || event.code == sf::Keyboard::Key::Numpad2)
			_eventManager.Emit(UpgradeSelected{ 2 });
		else if (event.code == sf::Keyboard::Key::Num3 || event.code == sf::Keyboard::Key::Numpad3)
			_eventManager.Emit(UpgradeSelected{ 3 });
		break;
	default:
		break;
	}

}

void Game::HandleEvent(const sf::Event&) {}//other events

void Game::ProcessEvents()
{
	_eventManager.Process([](const auto& event)
		{
			using T = std::decay_t<decltype(event)>;

			if constexpr (std::is_same_v<T, GameStateChange>)
			{
				_gameState = event.gameState;
				_gameWorld.GetUIManager().ActivateOverlay(event.gameState);

				if (event.gameState == Paused)
					_gameWorld.GetUIManager().OnPauseEnter();
			}
			else if constexpr (std::is_same_v<T, LevelUp>)
			{
				if (_gameWorld.HasPendingUpgrade())
				{
					_gameWorld.GetUIManager().OnUpgradeEnter(_gameWorld.GetPlayerUpgradeOptions());
					_eventManager.Emit(GameStateChange{ Upgrading });
				}
			}
			else if constexpr (std::is_same_v<T, UpgradeSelected>)
			{
				_gameWorld.ApplyUpgradeOption(event.option);
				if (!_gameWorld.HasPendingUpgrade())
					_eventManager.Emit(GameStateChange{Playing});
			}
			else if constexpr (std::is_same_v<T, PlayerDamaged>)
			{
				
			}
			else if constexpr (std::is_same_v<T, PlayerShoot>)
			{

			}
			else if constexpr (std::is_same_v<T, EnemyDamaged>)
			{

			}
			else if constexpr (std::is_same_v<T, EnemyKilled>)
			{
				
			}

		});
}

void Game::Start()
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({ 1024, 768 }, 32), "LightRogue");
	_gameState = GameState::ShowingSplash;
	_mainWindow.setVerticalSyncEnabled(true);
	_mainWindow.setKeyRepeatEnabled(false);
	_gameWorld.GetUIManager().LoadOverlayResources();
	_gameWorld.GetUIManager().OnSplashEnter();
	_gameWorld.GetUIManager().ActivateOverlay(GameState::ShowingSplash);

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
	return _gameState == GameState::Exiting ? true : false;
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
		{
			_gameWorld.GetUIManager().OnGameOverEnter(
				_gameWorld.GetGameTime(),
				_gameWorld.GetScore(),
				_gameWorld.GetPlayerLevel());
			_gameWorld.GetUIManager().ActivateOverlay(GameState::GameOver);
			_gameState = GameOver;
		}
	}

	ProcessEvents();

	_mainWindow.clear();
	if (_gameState == ShowingSplash)
		_gameWorld.GetUIManager().DrawOverlay(_mainWindow);
	else
	{
		_gameWorld.DrawAll(_mainWindow);
		if (_gameState != Playing)
			_gameWorld.GetUIManager().DrawOverlay(_mainWindow);
	}
	UpdateWindowTitle();
	_mainWindow.display();
}

void Game::BeginPlaying()
{
	_gameState = Playing;
	_clock.restart();
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

EventManager& Game::GetEventManager()
{
	return _eventManager;
}

GameState Game::_gameState = GameState::Uninitialized;
sf::RenderWindow Game::_mainWindow;
sf::Clock Game::_clock;
GameWorld Game::_gameWorld;
EventManager Game::_eventManager;
