#include "TimeManager.h"
#include "GameEngine.h"
#include "Constants.h"
#include <chrono>

// Runs in a seperate thread and keeps track of the time
void clockTask(std::stop_token stopToken, GameEngine* engine) {
	Event tick(EventId::CLK); // Event object to use for notification

	int sleep_duration{ 0 };
	while (!stopToken.stop_requested()) {
		this_thread::sleep_for(chrono::milliseconds(GAME_TICK));
		engine->notify(tick);
		engine->getState().tick;
	}
}

// Starts a seperate clock thread
void TimeManager::startClock() {
	BaseClock = std::jthread(clockTask, engine);
}

// Terminates the clock thread
void TimeManager::stopClock() {
	if (BaseClock.joinable()) {
		BaseClock.request_stop();
		BaseClock.join();
	}
}