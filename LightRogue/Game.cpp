#include"stdafx.h"
#include"GameObject.h"
#include"Game.h"
#include"ResourceLoader.h"
#include "SaveManager.h"

void Game::HandleEvent(const sf::Event::Closed&)
{
	_eventManager.Emit(GameStateChange{ Exiting });
}

void Game::HandleEvent(const sf::Event::MouseMoved& event)
{
	_gameWorld.GetUIManager().UpdateOverlayHover(_mainWindow, ToWorld(event.position));
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed& event)
{
	int action = _gameWorld.GetUIManager().HitTestOverlay(_mainWindow, ToWorld(event.position));

	switch (_gameState)
	{
	case ShowingSplash:
		if (action == 1) BeginPlaying();
		if (action == 2) _eventManager.Emit(LoadRequested{});
		break;
	case Paused:
		if (action == 1) _eventManager.Emit(GameStateChange{ Playing });
		if (action == 2) _eventManager.Emit(SaveRequested{});
		if (action == 3) _eventManager.Emit(GameStateChange{ Exiting });
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

void Game::HandleEvent(const sf::Event::Resized& event)//designed with AI assistance
{
	float windowW = static_cast<float>(event.size.x);
	float windowH = static_cast<float>(event.size.y);
	float worldRatio = 1440.f / 810.f;
	float windowRatio = windowW / windowH;

	float vpLeft = 0.f, vpTop = 0.f, vpW = 1.f, vpH = 1.f;

	if (windowRatio > worldRatio)
	{
		vpW = worldRatio / windowRatio;
		vpLeft = (1.f - vpW) / 2.f;
	}
	else
	{
		vpH = windowRatio / worldRatio;
		vpTop = (1.f - vpH) / 2.f;
	}

	_view.setViewport(sf::FloatRect({ vpLeft, vpTop }, { vpW, vpH }));
	_mainWindow.setView(_view);
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

				if (event.gameState == Playing)
					_gameWorld.GetSoundManager().PlayBGM();
				else if(event.gameState == Paused)
					_gameWorld.GetSoundManager().PauseBGM();
				else if (event.gameState == GameOver)
					_gameWorld.GetSoundManager().StopBGM();
			}
			else if constexpr (std::is_same_v<T, LevelUp>)
			{
				if (_gameWorld.HasPendingUpgrade())
				{
					_gameWorld.GetUIManager().OnUpgradeEnter(_gameWorld.GetPlayerUpgradeOptions());
					_eventManager.Emit(GameStateChange{ Upgrading });
				}
				_gameWorld.GetSoundManager().PlayLevelUp();
			}
			else if constexpr (std::is_same_v<T, UpgradeSelected>)
			{
				_gameWorld.ApplyUpgradeOption(event.option);
				if (!_gameWorld.HasPendingUpgrade())
					_eventManager.Emit(GameStateChange{Playing});
			}
			else if constexpr (std::is_same_v<T, PlayerDamaged>)
			{
				_gameWorld.GetSoundManager().PlayPlayerDamaged();
			}
			else if constexpr (std::is_same_v<T, PlayerShoot>)
			{
				_gameWorld.GetSoundManager().PlayPlayerShoot();
			}
			else if constexpr (std::is_same_v<T, EnemyDamaged>)
			{
				_gameWorld.GetSoundManager().PlayEnemyDamaged();
			}
			else if constexpr (std::is_same_v<T, EnemyKilled>)
			{
				
			}
			else if constexpr (std::is_same_v<T, SaveRequested>)
			{
				_gameWorld.SaveGame();
			}
			else if constexpr (std::is_same_v<T, LoadRequested>) 
			{
				if (_gameWorld.LoadGame())
					BeginPlaying();
			}


		});
}

sf::Vector2i Game::ToWorld(sf::Vector2i pixel)//designed with AI assistance
{
	sf::Vector2f f = _mainWindow.mapPixelToCoords(pixel);
	return { static_cast<int>(f.x), static_cast<int>(f.y) };
}

void Game::Start()
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode({ 1440, 810 }), "LightRogue");
	_gameState = GameState::ShowingSplash;
	_view = sf::View(sf::FloatRect({ 0.f, 0.f }, { 1440.f, 810.f }));
	_mainWindow.setView(_view);
	_mainWindow.setVerticalSyncEnabled(true);
	_mainWindow.setKeyRepeatEnabled(false);
	_gameWorld.GetUIManager().LoadOverlayResources();
	_gameWorld.GetSoundManager().LoadAll();
	_gameWorld.GetUIManager().OnSplashEnter();
	_gameWorld.GetUIManager().SetSplashHasSaveFile(SaveManager::SaveExist());
	_gameWorld.GetUIManager().ActivateOverlay(GameState::ShowingSplash);

	_gameWorld.GetSoundManager().PlayBGM();

	_clock.restart();
	
	//Create a player
	sf::Texture playerTexture;
	LoadTextureFromResource(playerTexture, "player.png");
	std::unique_ptr<Player> player = std::make_unique<Player>(playerTexture,&_gameWorld,&_mainWindow);
	player->SetPosition(720.f, 405.f);
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
	float FPS = 1.f / deltaTime;
	if (_smoothFPS)_smoothFPS = _smoothFPS * 0.9f + FPS * 0.1f;
	else _smoothFPS = FPS;

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
		_gameWorld.GetUIManager().DrawFPS(_mainWindow, static_cast<int>(_smoothFPS));
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
		_mainWindow.setTitle("LightRogue - Game Over | Any Key: exit");
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
sf::View Game::_view;
float Game::_smoothFPS = 0.f;