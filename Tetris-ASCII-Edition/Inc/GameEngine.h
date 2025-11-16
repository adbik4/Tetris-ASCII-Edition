#pragma once

#include <memory>
#include <iostream>
#include "Events.h"
#include "Settings.h"

using namespace std;

class TimeManager;

class Mediator {
	public:
		virtual void notify(const Event& event) = 0;
		virtual ~Mediator() = default;
};

// The GameEngine is a Mediator class, who keeps track of everything that is happening in the game.
// It notifies everyone about certain events who needs to react to them and decides on the order of operations.
class GameEngine : public Mediator {
private:
	unique_ptr<TimeManager> tm;
	uint8_t level;

public:
	GameEngine(const GameSettings& cfg) : level(cfg.start_level) {};
	~GameEngine() override {
		stopGame();
	}

	void setTimeManager(unique_ptr<TimeManager> ptr) { tm = move(ptr); };
	void notify(const Event& event) override;

	void startGame();
	void stopGame();

	uint8_t getLevel() const { return level; };
};