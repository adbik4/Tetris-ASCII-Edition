#pragma once
#include <cstdint>
#include <tuple>

using namespace std;

enum class EventId : int8_t {
	CLK = 0,
};

typedef struct {
	EventId id;
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