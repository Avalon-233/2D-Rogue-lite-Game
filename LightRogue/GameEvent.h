#pragma once
#include<queue>
#include<variant>

enum GameState { Uninitialized, ShowingSplash, Paused, ShowingMenu, Playing, Exiting, GameOver, Upgrading };

struct LevelUp { int newlevel; };
struct UpgradeSelected { int option; };
struct MouseClicked { int x;int y; GameState gameState; };
struct GameStateChange { GameState gameState; };
struct PlayerDamaged {};
struct EnemyKilled {};

using GameEvent = std::variant<LevelUp, MouseClicked, GameStateChange,UpgradeSelected, PlayerDamaged, EnemyKilled>;

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