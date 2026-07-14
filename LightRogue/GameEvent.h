#pragma once
#include<queue>
#include<variant>

enum GameState { Uninitialized, ShowingSplash, Paused, ShowingMenu, Playing, Exiting, GameOver, Upgrading };

struct LevelUp { int newlevel; };
struct UpgradeSelected { int option; };
struct GameStateChange { GameState gameState; };
struct PlayerDamaged {};
struct PlayerShoot {};
struct EnemyDamaged {};
struct EnemyKilled {};

using GameEvent = std::variant<LevelUp, GameStateChange,UpgradeSelected, PlayerDamaged, PlayerShoot, EnemyDamaged, EnemyKilled>;

class EventManager
{
public:
	void Emit(const GameEvent& event)
	{
		_eventQueue.push(event);
	}

	template<typename F>
	void Process(F&& handler)
	{
		while (!_eventQueue.empty())
		{
			std::visit(handler, _eventQueue.front());
			_eventQueue.pop();
		}
	}

private:
	std::queue<GameEvent> _eventQueue;
};