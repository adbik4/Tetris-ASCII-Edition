#include "TimeManager.h"

#include <thread>
#include "GameEngine.h"

void clockTask(GameEngine* engine) {
	Event tick(CLK); // Event object to use for notification

	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1000));
		engine->notify(tick);
	}
}

void TimeManager::startClock() {
	thread t(clockTask, engine);
	t.detach();
}