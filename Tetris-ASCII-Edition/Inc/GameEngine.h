#pragma once
#include <ProjectHeaders.h>
#include "Events.h"
using namespace std;

// The GameEngine is a Mediator class, who keeps track of everything that is happening in the game.
// It notifies everyone about certain events who needs to react to them and decides on the order of operations.

class TimeManager;

class Mediator {
	public:
		virtual void notify(const Event& event) = 0;
		virtual ~Mediator() = default;
};

class GameEngine : public Mediator {
private:
	unique_ptr<TimeManager> clock;

public:
	void setTimeManager(unique_ptr<TimeManager> ptr) { clock = move(ptr); };
	void notify(const Event& event) override;

	void startGame();
	void stopGame();
};