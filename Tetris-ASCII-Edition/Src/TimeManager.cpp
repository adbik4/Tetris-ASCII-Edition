#include "TimeManager.h"
#include "GameEngine.h"
#include "Constants.h"
#include <chrono>

// Runs in a seperate thread and keeps track of the time
// Args: stopToken - std::stop_token to cooperatively end the thread; engine - GameEngine* to notify
// Returns: void
void clockTask(std::stop_token stopToken, GameEngine* engine) {
	int sleep_duration{ 0 };
	while (!stopToken.stop_requested()) {
		this_thread::sleep_for(chrono::milliseconds(GAME_TICK));
		engine->notify(Event::CLK);
		engine->getState().tick;
	}
}

// Starts a seperate clock thread
// Args: none (uses associated engine pointer)
// Returns: void
void TimeManager::startClock() {
	BaseClock = std::jthread(clockTask, engine);
}

// Terminates the clock thread
// Args: none
// Returns: void
void TimeManager::stopClock() {
	if (BaseClock.joinable()) {
		BaseClock.request_stop();
		BaseClock.join();
	}
}