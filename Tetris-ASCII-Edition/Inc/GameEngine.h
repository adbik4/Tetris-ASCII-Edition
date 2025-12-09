#pragma once

#include <memory>
#include "Events.h"
#include "GameState.h"
#include "GameRenderer.h"
#include "TimeManager.h"
#include "InputManager.h"

// TODO: create a parent class for ncurses io classes
// contains pointers to each window and getters

using namespace std;

class TimeManager;
class InputManager;
class GameRenderer;

class Mediator {
	public:
		virtual void notify(const Event& event) = 0;
		virtual ~Mediator() = default;
};

// The GameEngine is a Mediator class, which keeps track of everything that is happening in the game.
// It notifies everyone about certain events who needs to react to them and decides on the order of operations.
class GameEngine : public Mediator {
private:
	unique_ptr<GameRenderer> renderer;
	unique_ptr<TimeManager> time_mgr;
	unique_ptr<InputManager> input_mgr;

	GameState state;

public:
	GameEngine(const GameSettings& cfg) : state(cfg) {};

	void setTimeManager(unique_ptr<TimeManager> ptr) { time_mgr = move(ptr); }
	void setGameRenderer(unique_ptr<GameRenderer> ptr) { renderer = move(ptr); }
	void setInputManager(unique_ptr<InputManager> ptr) { input_mgr = move(ptr); }
	void notify(const Event& event) override;

	void startEngine();
	void update();
	void stopEngine();

	void nextLevel();

	GameState getState() const { return state; }
};