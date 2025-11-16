#include "TimeManager.h"

#include "GameEngine.h"

void clockTask(std::stop_token stopToken, GameEngine* engine) {
	Event tick(CLK); // Event object to use for notification

	while (!stopToken.stop_requested()) {
		this_thread::sleep_for(chrono::milliseconds(1000));
		engine->notify(tick);
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