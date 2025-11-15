#pragma once
#include "ProjectHeaders.h"
#include <chrono>
using namespace std;

class GameEngine;

class TimeManager {
private:
	shared_ptr<GameEngine> engine;
	uint8_t level;

public:
	TimeManager(const shared_ptr<GameEngine>& ptr, const uint8_t start_level = 0) : engine(ptr), level(start_level) {};

	void startClock();
};