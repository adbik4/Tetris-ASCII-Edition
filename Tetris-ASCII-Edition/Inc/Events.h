#pragma once
#include <cstdint>
#include <tuple>

using namespace std;

enum class EventId : int8_t {
	INPUT_ERR = -2,
	GENERAL_ERR = -1,
	CLK = 0,
	INT_INPUT = 1
};

typedef struct {
	EventId id;
	tuple<int, int> args;
} Event;

/*
class Event {
public:
	int id;
	int getId() { return id; }
};

class TickEvent : public Event {};

class InputEvent : public Event {
	tuple<int, int> args;
};
*/