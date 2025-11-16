#pragma once
#include "ProjectHeaders.h"
#include <chrono>
using namespace std;

class GameEngine;

class TimeManager {
	GameEngine* engine;
	uint8_t level;

public:
	TimeManager(GameEngine* ptr, const uint8_t start_level = 0) : engine(ptr), level(start_level) {}
	void startClock();
};