#pragma once
#include <thread>
#include <chrono>

using namespace std;

class GameEngine;

// The TimeManager launches and keeps track of threads that keep track of time.
// The threads will generate notifications that trigger certain actions in the GameEngine
class TimeManager {
	GameEngine* engine;
	std::jthread BaseClock;

public:
	TimeManager(GameEngine* ptr) : engine(ptr) {}
	void startClock();
	void stopClock();
};