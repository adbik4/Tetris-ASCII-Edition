#include "TimeManager.h"
#include "GameEngine.h"

void clockTask(std::stop_token stopToken, GameEngine* engine) {
	Event tick(CLK); // Event object to use for notification

	int sleep_duration{ 0 };
	while (!stopToken.stop_requested()) {
		sleep_duration = static_cast<int>( 1000.0 / pow(2, engine->getLevel()) );
		this_thread::sleep_for(chrono::milliseconds(sleep_duration));
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