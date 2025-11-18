#pragma once

#include <memory>
#include "Events.h"
#include "Settings.h"
#include "GameRenderer.h"
#include "TimeManager.h"
#include "InputManager.h"

class TimeManager;

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

	bool running_ = false;	// used to check if the engine systems are running
	bool stop_flag = false; // triggers the app shutdown
	uint8_t level_;

public:
	GameEngine(const GameSettings& cfg) : level_(cfg.start_level) {};

	void setTimeManager(unique_ptr<TimeManager> ptr) { time_mgr = move(ptr); }
	void setGameRenderer(unique_ptr<GameRenderer> ptr) { renderer = move(ptr); }
	void setInputManager(unique_ptr<InputManager> ptr) { input_mgr = move(ptr); }
	void notify(const Event& event) override;

	void startGame();
	void update();
	void stopGame();

	void nextLevel();

	uint8_t getLevel() const { return level_; }
	bool getStatus() const { return stop_flag; }
};