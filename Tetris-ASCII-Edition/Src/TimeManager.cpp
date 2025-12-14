#include "TimeManager.h"
#include "GameEngine.h"
#include "Constants.h"

void clockTask(std::stop_token stopToken, GameEngine* engine) {
	Event tick(CLK, {0, 0}); // Event object to use for notification

	int sleep_duration{ 0 };
	while (!stopToken.stop_requested()) {
		this_thread::sleep_for(chrono::milliseconds(GAME_TICK));
		engine->notify(tick);
		engine->getState().tick;
	}
}

void TimeManager::startClock() {
	BaseClock = std::jthread(clockTask, engine);
}

void TimeManager::stopClock() {
	if (BaseClock.joinable()) {
		BaseClock.request_stop();
		BaseClock.join();
	}
}